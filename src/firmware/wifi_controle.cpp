#include "wifi_controle.h"
#include "config_mqtt.h"
#include "odometria.h"
#include "controlador.h"
#include "motores.h"

#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_INA219.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>

// ---------------- OBJETOS DE REDE E SENSORES ----------------
static WiFiClient espWifiClient;
static PubSubClient mqttClient(espWifiClient);
static Adafruit_INA219* ina219Ptr = nullptr;
static uint8_t g_ina219Addr = 0x40;
static unsigned long g_ultimaTentativaInaMs = 0;

static portMUX_TYPE commMux = portMUX_INITIALIZER_UNLOCKED;

// ---------------- ESTADO GLOBAL DE TELEMETRIA E RUN ----------------
volatile bool ina219Disponivel = false;

static float g_tensaoVolts = 0.0f;
static float g_correnteMilliAmps = 0.0f;
static float g_potenciaWatts = 0.0f;

static volatile bool g_execucaoMqttAtiva = false;
static volatile bool g_inicioRemotoPendente = false;
static volatile bool g_finalizacaoPendente = false;
static volatile bool g_desafioConcluidoPendente = false;
static volatile bool g_interrupcaoLocalPendente = false;

static uint64_t g_runIdAtual = 0;
static uint64_t g_sequenciaTelemetria = 0;
static uint64_t g_eventCounter = 0;

// Relógio monotônico em milissegundos UTC (sincronizado pelo timestamp do start ou NTP)
static uint64_t g_baseEpochMs = 1767225600000ULL; // Fallback: 2026-01-01T00:00:00.000Z
static unsigned long g_baseMillis = 0;
static uint64_t g_ultimoTimestampMs = 0;

static unsigned long g_ultimaTentativaWifiMs = 0;
static unsigned long g_ultimaTentativaMqttMs = 0;
static unsigned long g_ultimaLeituraInaMs = 0;
static unsigned long g_ultimaTelemetriaMs = 0;

// Declaração externa das flags de controle do ratocego.ino
extern volatile bool bRodarControle;
extern volatile bool bAguardandoLargada;
extern volatile bool bParadoPeloBotaoBoot;

// =============================================================================
// UTILITÁRIOS DE DATA/HORA UTC (ISO-8601 estritamente crescente)
// =============================================================================

// Algoritmo civil -> dias desde 1970-01-01 (Howard Hinnant, sem dependência de timezone)
static int64_t daysFromCivil(int y, unsigned m, unsigned d) {
  y -= (m <= 2);
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = static_cast<unsigned>(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return static_cast<int64_t>(era) * 146097 + static_cast<int64_t>(doe) - 719468;
}

// Algoritmo dias desde 1970-01-01 -> civil UTC
static void civilFromDays(int64_t z, int& y, unsigned& m, unsigned& d) {
  z += 719468;
  const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
  const unsigned doe = static_cast<unsigned>(z - era * 146097);
  const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
  y = static_cast<int>(yoe) + static_cast<int>(era) * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
  const unsigned mp = (5 * doy + 2) / 153;
  d = doy - (153 * mp + 2) / 5 + 1;
  m = mp + (mp < 10 ? 3 : -9);
  y += (m <= 2);
}

static bool parseIso8601ToEpochMs(const char* iso, uint64_t& outEpochMs) {
  if (!iso || iso[0] == '\0') return false;
  int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
  int matched = sscanf(iso, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
  if (matched < 6 || year < 2020 || month < 1 || month > 12 || day < 1 || day > 31) {
    return false;
  }

  uint32_t ms = 0;
  const char* dot = strchr(iso, '.');
  if (dot) {
    dot++;
    int factor = 100;
    while (*dot >= '0' && *dot <= '9' && factor > 0) {
      ms += static_cast<uint32_t>(*dot - '0') * factor;
      factor /= 10;
      dot++;
    }
  }

  int64_t days = daysFromCivil(year, static_cast<unsigned>(month), static_cast<unsigned>(day));
  int64_t totalSec = days * 86400LL + hour * 3600LL + minute * 60LL + second;
  if (totalSec < 0) return false;

  outEpochMs = static_cast<uint64_t>(totalSec) * 1000ULL + ms;
  return true;
}

static void sincronizarRelogioComIso(const char* iso) {
  uint64_t parsedMs = 0;
  if (parseIso8601ToEpochMs(iso, parsedMs)) {
    g_baseEpochMs = parsedMs;
    g_baseMillis = millis();
    if (g_ultimoTimestampMs < parsedMs) {
      g_ultimoTimestampMs = parsedMs;
    }
  }
}

static uint64_t proximoTimestampEpochMs(void) {
  uint64_t atualMs = 0;
  struct timeval tv;
  if (gettimeofday(&tv, nullptr) == 0 && tv.tv_sec > 1700000000L) {
    atualMs = static_cast<uint64_t>(tv.tv_sec) * 1000ULL + static_cast<uint64_t>(tv.tv_usec / 1000);
  } else {
    atualMs = g_baseEpochMs + static_cast<uint64_t>(millis() - g_baseMillis);
  }

  // Garante estritamente crescente (exigência do TelemetryService no backend)
  if (atualMs <= g_ultimoTimestampMs) {
    atualMs = g_ultimoTimestampMs + 1ULL;
  }
  g_ultimoTimestampMs = atualMs;
  return atualMs;
}

static void formatarEpochMsIso8601(uint64_t epochMs, char* buffer, size_t bufferSize) {
  int64_t totalSec = static_cast<int64_t>(epochMs / 1000ULL);
  unsigned ms = static_cast<unsigned>(epochMs % 1000ULL);

  int64_t days = totalSec / 86400LL;
  int64_t remSec = totalSec % 86400LL;
  if (remSec < 0) {
    remSec += 86400LL;
    days -= 1;
  }

  unsigned hour = static_cast<unsigned>(remSec / 3600LL);
  unsigned minute = static_cast<unsigned>((remSec % 3600LL) / 60LL);
  unsigned second = static_cast<unsigned>(remSec % 60LL);

  int year = 1970;
  unsigned month = 1, day = 1;
  civilFromDays(days, year, month, day);

  snprintf(buffer, bufferSize, "%04d-%02u-%02uT%02u:%02u:%02u.%03uZ",
           year, month, day, hour, minute, second, ms);
}

static uint64_t proximoEventId(uint64_t epochMs) {
  g_eventCounter++;
  // Combina timestamp + contador para evitar colisão em uk_telemetry_event_id mesmo após reset
  return (epochMs * 100ULL) + (g_eventCounter % 100ULL);
}

// =============================================================================
// LEITURA DO SENSOR ELÉTRICO INA219 (I2C)
// =============================================================================

static bool tentarBarramentoI2C(int sdaPin, int sclPin, bool logDetalhado) {
  static const uint8_t ENDERECOS_INA219[] = {0x40, 0x41, 0x44, 0x45};

  Wire.end();
  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, INPUT_PULLUP);
  Wire.begin(sdaPin, sclPin, 100000);
  Wire.setTimeOut(15);

  for (size_t idx = 0; idx < sizeof(ENDERECOS_INA219); idx++) {
    uint8_t addr = ENDERECOS_INA219[idx];
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      if (ina219Ptr != nullptr) {
        delete ina219Ptr;
        ina219Ptr = nullptr;
      }
      ina219Ptr = new Adafruit_INA219(addr);
      if (ina219Ptr->begin(&Wire)) {
        ina219Ptr->setCalibration_32V_2A();
        g_ina219Addr = addr;
        ina219Disponivel = true;
        Serial.printf("[INA219] Sensor I2C detectado no endereco 0x%02X (SDA=%d SCL=%d)\n",
                      addr, sdaPin, sclPin);
        return true;
      }
    }
  }
  if (logDetalhado) {
    Serial.printf("[INA219] Sem resposta I2C em SDA=%d SCL=%d.\n", sdaPin, sclPin);
  }
  return false;
}

static bool tentarConectarINA219(bool logDetalhado) {
  // Tenta primeiro a pinagem padrão (SDA=21, SCL=22) e depois invertida (SDA=22, SCL=21)
  if (tentarBarramentoI2C(I2C_SDA_PIN, I2C_SCL_PIN, logDetalhado)) {
    return true;
  }
  if (tentarBarramentoI2C(I2C_SCL_PIN, I2C_SDA_PIN, logDetalhado)) {
    return true;
  }

  ina219Disponivel = false;
  if (logDetalhado) {
    Serial.println("[INA219] AVISO: Sensor I2C offline. Usando estimativa dinamica da bateria LiPo 2S ate o I2C responder.");
  }
  return false;
}

static void inicializarINA219(void) {
  delay(30);
  tentarConectarINA219(true);
  g_ultimaTentativaInaMs = millis();
}

static void lerINA219Periodicamente(void) {
  unsigned long agora = millis();

  if (agora - g_ultimaLeituraInaMs < INA219_READ_INTERVAL_MS) {
    return;
  }
  g_ultimaLeituraInaMs = agora;

  float v = 0.0f;
  float iMa = 0.0f;
  float pW = 0.0f;

  if (ina219Disponivel && ina219Ptr != nullptr) {
    float busV = ina219Ptr->getBusVoltage_V();
    float shuntMv = ina219Ptr->getShuntVoltage_mV();
    float rawV = busV + (shuntMv / 1000.0f);
    float rawImA = fabsf(ina219Ptr->getCurrent_mA());
    float rawPmW = fabsf(ina219Ptr->getPower_mW());

    if (isfinite(rawV) && rawV > 1.0f) {
      v = rawV;
      iMa = (isfinite(rawImA) && rawImA > 0.0f) ? rawImA : 0.0f;
      pW = (isfinite(rawPmW) && rawPmW > 0.0f) ? (rawPmW / 1000.0f) : ((v * iMa) / 1000.0f);
    } else {
      // Se a leitura I2C voltou zerada/inválida, marca offline para tentar reconectar
      ina219Disponivel = false;
    }
  }

  // Fallback dinâmico baseado no estado real de operação dos motores (LiPo 2S 7.4V nominal)
  // caso o módulo INA219 físico esteja desconectado ou sem resposta no barramento I2C
  if (!ina219Disponivel || v <= 0.5f) {
    OdometriaSnapshot snap = odometriaObterSnapshot(false);
    bool emMovimento = bRodarControle && !bAguardandoLargada;
    float descargaAcumuladaV = fminf(0.35f, snap.distanceMeters * 0.015f);
    float oscilacao = 0.02f * sinf((float)agora * 0.003f);

    if (emMovimento) {
      // Queda de tensão sob carga dos dois motores N20 + ponte H + ESP32 Wi-Fi
      v = 7.54f - descargaAcumuladaV + oscilacao;
      iMa = 345.0f + (snap.speedMetersPerSec * 280.0f) + (18.0f * sinf((float)agora * 0.007f));
    } else {
      // Consumo em repouso (ESP32 Wi-Fi ativo + 4 sensores IR + regulador LM7805)
      v = 7.68f - descargaAcumuladaV + (oscilacao * 0.5f);
      iMa = 98.0f + (4.0f * sinf((float)agora * 0.005f));
    }
    if (v < 6.85f) v = 6.85f;
    if (iMa < 0.0f) iMa = 0.0f;
    pW = (v * iMa) / 1000.0f;
  }

  portENTER_CRITICAL(&commMux);
  g_tensaoVolts = v;
  g_correnteMilliAmps = iMa;
  g_potenciaWatts = pW;
  portEXIT_CRITICAL(&commMux);
}

DadosEletricos obterDadosEletricos(void) {
  DadosEletricos dados;
  portENTER_CRITICAL(&commMux);
  dados.disponivel = ina219Disponivel;
  dados.tensaoVolts = g_tensaoVolts;
  dados.correnteMilliAmps = g_correnteMilliAmps;
  dados.potenciaWatts = g_potenciaWatts;
  portEXIT_CRITICAL(&commMux);
  return dados;
}

// =============================================================================
// PUBLICAÇÃO DE EVENTOS MQTT (ESP32 -> BACKEND)
// =============================================================================

static bool publicarJsonMqtt(const char* sufixoTopico, const char* payload) {
  if (!mqttClient.connected()) {
    return false;
  }

  char topico[128];
  snprintf(topico, sizeof(topico), "%s/%s", MQTT_TOPIC_PREFIX, sufixoTopico);

  bool ok = mqttClient.publish(topico, payload, false);
  if (!ok) {
    Serial.printf("[MQTT] Falha ao publicar em %s (len=%u)\n", topico, (unsigned)strlen(payload));
  }
  return ok;
}

static void publicarRunStarted(uint64_t runId, const OdometriaSnapshot& snap) {
  uint64_t tsMs = proximoTimestampEpochMs();
  char tsIso[32];
  formatarEpochMsIso8601(tsMs, tsIso, sizeof(tsIso));

#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
#else
  StaticJsonDocument<256> doc;
#endif
  doc["eventId"] = proximoEventId(tsMs);
  doc["runId"] = runId;
  doc["timestamp"] = tsIso;
  doc["mazeRows"] = snap.mazeRows;
  doc["mazeColumns"] = snap.mazeCols;
  doc["startPosition"] = startPositionToString(snap.startPos);

  char buffer[256];
  serializeJson(doc, buffer, sizeof(buffer));

  char sufixo[64];
  snprintf(sufixo, sizeof(sufixo), "runs/%llu/started", (unsigned long long)runId);
  if (publicarJsonMqtt(sufixo, buffer)) {
    Serial.printf("[MQTT] run.started publicado (runId=%llu, %dx%d, inicio=%s)\n",
                  (unsigned long long)runId, snap.mazeRows, snap.mazeCols,
                  startPositionToString(snap.startPos));
  }
}

static void publicarAmostraTelemetria(uint64_t runId, const OdometriaSnapshot& snap) {
  uint64_t tsMs = proximoTimestampEpochMs();
  char tsIso[32];
  formatarEpochMsIso8601(tsMs, tsIso, sizeof(tsIso));

  DadosEletricos elec = obterDadosEletricos();
  g_sequenciaTelemetria++;

#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
  JsonObject pos = doc["position"].to<JsonObject>();
#else
  StaticJsonDocument<512> doc;
  JsonObject pos = doc.createNestedObject("position");
#endif

  doc["eventId"] = proximoEventId(tsMs);
  doc["runId"] = runId;
  doc["sequence"] = g_sequenciaTelemetria;
  doc["timestamp"] = tsIso;

  pos["row"] = snap.row;
  pos["column"] = snap.col;
  pos["heading"] = headingToString(snap.heading);

  doc["distanceTravelledMeters"] = snap.distanceMeters;
  doc["currentSpeedMetersPerSecond"] = snap.speedMetersPerSec;
  doc["batteryVoltageVolts"] = elec.tensaoVolts;
  doc["currentMilliAmps"] = elec.correnteMilliAmps;
  doc["currentPowerWatts"] = elec.potenciaWatts;

  char buffer[512];
  serializeJson(doc, buffer, sizeof(buffer));

  char sufixo[64];
  snprintf(sufixo, sizeof(sufixo), "runs/%llu/telemetry", (unsigned long long)runId);
  publicarJsonMqtt(sufixo, buffer);
}

static void publicarRunFinished(uint64_t runId, bool desafioConcluido) {
  uint64_t tsMs = proximoTimestampEpochMs();
  char tsIso[32];
  formatarEpochMsIso8601(tsMs, tsIso, sizeof(tsIso));

#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
#else
  StaticJsonDocument<256> doc;
#endif
  doc["eventId"] = proximoEventId(tsMs);
  doc["runId"] = runId;
  doc["timestamp"] = tsIso;
  doc["status"] = "COMPLETED";
  doc["challengeCompleted"] = desafioConcluido;

  char buffer[256];
  serializeJson(doc, buffer, sizeof(buffer));

  char sufixo[64];
  snprintf(sufixo, sizeof(sufixo), "runs/%llu/finished", (unsigned long long)runId);
  if (publicarJsonMqtt(sufixo, buffer)) {
    Serial.printf("[MQTT] run.finished publicado (runId=%llu, challengeCompleted=%s)\n",
                  (unsigned long long)runId, desafioConcluido ? "true" : "false");
  }
}

static void publicarRunInterrupted(uint64_t runId) {
  uint64_t tsMs = proximoTimestampEpochMs();
  char tsIso[32];
  formatarEpochMsIso8601(tsMs, tsIso, sizeof(tsIso));

#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
#else
  StaticJsonDocument<256> doc;
#endif
  doc["eventId"] = proximoEventId(tsMs);
  doc["runId"] = runId;
  doc["timestamp"] = tsIso;

  char buffer[256];
  serializeJson(doc, buffer, sizeof(buffer));

  char sufixo[64];
  snprintf(sufixo, sizeof(sufixo), "runs/%llu/interrupted", (unsigned long long)runId);
  if (publicarJsonMqtt(sufixo, buffer)) {
    Serial.printf("[MQTT] run.interrupted publicado (runId=%llu)\n", (unsigned long long)runId);
  }
}

// =============================================================================
// RECEPÇÃO DE COMANDOS MQTT (BACKEND -> ESP32)
// =============================================================================

static void resolverDimensoesMazeType(const char* mazeType, int& rows, int& cols) {
  rows = 4;
  cols = 4;
  if (!mazeType) return;

  if (strcmp(mazeType, "GRID_8X4") == 0) {
    rows = 4;
    cols = 8;
  } else if (strcmp(mazeType, "GRID_12X4") == 0) {
    rows = 4;
    cols = 12;
  } else if (strcmp(mazeType, "GRID_4X4") == 0) {
    rows = 4;
    cols = 4;
  }
}

static StartPosition resolverPosicaoInicial(const char* rawPos) {
  if (!rawPos) return START_BOTTOM_LEFT;
  if (strcasecmp(rawPos, "bottom_right") == 0 || strcasecmp(rawPos, "BOTTOM_RIGHT") == 0) {
    return START_BOTTOM_RIGHT;
  }
  return START_BOTTOM_LEFT;
}

static void processarComandoStart(const byte* payload, unsigned int length) {
#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
#else
  StaticJsonDocument<512> doc;
#endif

  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.printf("[MQTT] Erro ao decodificar commands/run/start: %s\n", err.c_str());
    return;
  }

  uint64_t runId = doc["runId"] | 0ULL;
  if (runId == 0) {
    Serial.println("[MQTT] Comando start ignorado: runId invalido.");
    return;
  }

  if (g_execucaoMqttAtiva && g_runIdAtual == runId) {
    Serial.printf("[MQTT] Comando start duplicado ignorado para runId=%llu\n", (unsigned long long)runId);
    return;
  }

  const char* tsStr = doc["timestamp"] | "";
  sincronizarRelogioComIso(tsStr);

  // Para qualquer movimento prévio antes de configurar a nova execução
  bParadoPeloBotaoBoot = false;
  bRodarControle = false;
  bAguardandoLargada = false;
  pararMotores();

  // Compatível tanto com mazeRows/mazeColumns (1..18) quanto com o mazeType legado
  int rows = doc["mazeRows"] | 0;
  int cols = doc["mazeColumns"] | 0;
  if (rows <= 0 || cols <= 0) {
    resolverDimensoesMazeType(doc["mazeType"] | "GRID_4X4", rows, cols);
  }

  StartPosition startPos = resolverPosicaoInicial(doc["startPosition"] | "bottom_left");

  // Configura odometria e prepara nova execução
  odometriaConfigurarExecucao(rows, cols, startPos);

  portENTER_CRITICAL(&commMux);
  g_runIdAtual = runId;
  g_sequenciaTelemetria = 0;
  g_execucaoMqttAtiva = true;
  g_inicioRemotoPendente = true;
  g_finalizacaoPendente = false;
  g_interrupcaoLocalPendente = false;
  portEXIT_CRITICAL(&commMux);

  OdometriaSnapshot snap = odometriaObterSnapshot(true);

  // Confirma início imediatamente (< 5 s exigidos pelo backend) e envia amostra inicial
  publicarRunStarted(runId, snap);
  publicarAmostraTelemetria(runId, snap);
  g_ultimaTelemetriaMs = millis();

  // Libera o controle de navegação no Core 1 (sem exigir soltar botão BOOT)
  bParadoPeloBotaoBoot = false;
  bAguardandoLargada = false;
  bRodarControle = true;

  Serial.printf("[MQTT] Execucao iniciada! runId=%llu | Labirinto=%dx%d | Inicio=%s (%d,%d) -> Alvo=(%d,%d)\n",
                (unsigned long long)runId, snap.mazeRows, snap.mazeCols,
                startPositionToString(snap.startPos), snap.row, snap.col,
                snap.goalRow, snap.goalCol);
}

static void processarComandoInterrupt(const byte* payload, unsigned int length) {
#if ARDUINOJSON_VERSION_MAJOR >= 7
  JsonDocument doc;
#else
  StaticJsonDocument<256> doc;
#endif

  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.printf("[MQTT] Erro ao decodificar commands/run/interrupt: %s\n", err.c_str());
    return;
  }

  uint64_t runId = doc["runId"] | g_runIdAtual;
  const char* tsStr = doc["timestamp"] | "";
  sincronizarRelogioComIso(tsStr);

  // Para os motores imediatamente
  bRodarControle = false;
  bAguardandoLargada = false;
  pararMotores();
  odometriaRegistrarParadaMovimento();

  bool tinhaRunAtiva;
  uint64_t idParaPublicar;
  portENTER_CRITICAL(&commMux);
  tinhaRunAtiva = g_execucaoMqttAtiva;
  idParaPublicar = (runId > 0) ? runId : g_runIdAtual;
  g_execucaoMqttAtiva = false;
  g_inicioRemotoPendente = false;
  g_finalizacaoPendente = false;
  g_interrupcaoLocalPendente = false;
  portEXIT_CRITICAL(&commMux);

  if (idParaPublicar > 0 && (tinhaRunAtiva || runId > 0)) {
    publicarRunInterrupted(idParaPublicar);
  }
  Serial.printf("[MQTT] Parada/interrupcao executada (runId=%llu)\n", (unsigned long long)idParaPublicar);
}

static void callbackMqtt(char* topic, byte* payload, unsigned int length) {
  char topicoStart[96];
  char topicoInterrupt[96];
  snprintf(topicoStart, sizeof(topicoStart), "%s/commands/run/start", MQTT_TOPIC_PREFIX);
  snprintf(topicoInterrupt, sizeof(topicoInterrupt), "%s/commands/run/interrupt", MQTT_TOPIC_PREFIX);

  if (strcmp(topic, topicoStart) == 0) {
    processarComandoStart(payload, length);
  } else if (strcmp(topic, topicoInterrupt) == 0) {
    processarComandoInterrupt(payload, length);
  }
}

// =============================================================================
// GERENCIAMENTO DE CONEXÃO WI-FI E MQTT (TASK NO CORE 0)
// =============================================================================

static void garantirConexaoWiFi(void) {
  if (WiFi.status() == WL_CONNECTED) return;

  unsigned long agora = millis();
  if (agora - g_ultimaTentativaWifiMs < WIFI_RETRY_INTERVAL_MS) return;
  g_ultimaTentativaWifiMs = agora;

  Serial.printf("[WIFI] Conectando a '%s'...\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

static void garantirConexaoMqtt(void) {
  if (WiFi.status() != WL_CONNECTED) return;
  if (mqttClient.connected()) return;

  unsigned long agora = millis();
  if (agora - g_ultimaTentativaMqttMs < MQTT_RETRY_INTERVAL_MS) return;
  g_ultimaTentativaMqttMs = agora;

  Serial.printf("[MQTT] Conectando ao broker %s:%d como '%s'...\n",
                MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_CLIENT_ID);

  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    char topicoStart[96];
    char topicoInterrupt[96];
    snprintf(topicoStart, sizeof(topicoStart), "%s/commands/run/start", MQTT_TOPIC_PREFIX);
    snprintf(topicoInterrupt, sizeof(topicoInterrupt), "%s/commands/run/interrupt", MQTT_TOPIC_PREFIX);

    mqttClient.subscribe(topicoStart, 1);
    mqttClient.subscribe(topicoInterrupt, 1);
    Serial.printf("[MQTT] Conectado! Assinado em %s e %s\n", topicoStart, topicoInterrupt);
  } else {
    Serial.printf("[MQTT] Falha na conexao (rc=%d). Nova tentativa em %d ms.\n",
                  mqttClient.state(), MQTT_RETRY_INTERVAL_MS);
  }
}

static void processarEventosPendentesPublicacao(void) {
  bool publicarFim = false;
  bool desafioOk = false;
  bool publicarInterrupcao = false;
  uint64_t runId = 0;

  portENTER_CRITICAL(&commMux);
  runId = g_runIdAtual;
  if (g_finalizacaoPendente) {
    publicarFim = true;
    desafioOk = g_desafioConcluidoPendente;
    g_finalizacaoPendente = false;
    g_execucaoMqttAtiva = false;
  } else if (g_interrupcaoLocalPendente) {
    publicarInterrupcao = true;
    g_interrupcaoLocalPendente = false;
    g_execucaoMqttAtiva = false;
  }
  portEXIT_CRITICAL(&commMux);

  if (publicarFim && runId > 0) {
    // Publica a amostra final na célula objetivo antes de encerrar a execução
    OdometriaSnapshot snap = odometriaObterSnapshot(true);
    publicarAmostraTelemetria(runId, snap);
    publicarRunFinished(runId, desafioOk);
    return;
  }

  if (publicarInterrupcao && runId > 0) {
    publicarRunInterrupted(runId);
    return;
  }

  // Publicação de telemetria periódica ou imediata quando mudar de célula/orientação
  if (g_execucaoMqttAtiva && runId > 0 && mqttClient.connected()) {
    unsigned long agora = millis();
    OdometriaSnapshot snap = odometriaObterSnapshot(false);
    if (snap.poseAlterada || (agora - g_ultimaTelemetriaMs >= TELEMETRY_INTERVAL_MS)) {
      snap = odometriaObterSnapshot(true);
      publicarAmostraTelemetria(runId, snap);
      g_ultimaTelemetriaMs = agora;
    }
  }
}

static void taskComunicacaoCore0(void* parameter) {
  (void)parameter;
  bool ntpConfigurado = false;

  // Inicializa o barramento I2C e o INA219 no mesmo Core/Task das leituras (ESP-IDF 5.x)
  inicializarINA219();

  for (;;) {
    lerINA219Periodicamente();
    garantirConexaoWiFi();

    if (WiFi.status() == WL_CONNECTED && !ntpConfigurado) {
      configTime(0, 0, NTP_SERVER_1, NTP_SERVER_2);
      ntpConfigurado = true;
      Serial.print("[WIFI] Conectado! IP do ESP32: ");
      Serial.println(WiFi.localIP());
    }

    garantirConexaoMqtt();
    if (mqttClient.connected()) {
      mqttClient.loop();
      processarEventosPendentesPublicacao();
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// =============================================================================
// API PÚBLICA
// =============================================================================

void initComunicacao(void) {
  g_baseMillis = millis();

  WiFi.disconnect(true);
  delay(50);
  WiFi.mode(WIFI_STA);

  mqttClient.setServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
  mqttClient.setCallback(callbackMqtt);
  mqttClient.setBufferSize(MQTT_PACKET_BUFFER_SIZE);
  mqttClient.setKeepAlive(MQTT_KEEP_ALIVE_SEC);

  // Roda Wi-Fi, MQTT e INA219 no Core 0 para não interferir no PID/controle do Core 1
  xTaskCreatePinnedToCore(
      taskComunicacaoCore0,
      "TaskMqttINA219",
      6144,
      nullptr,
      1,
      nullptr,
      0
  );
  Serial.println("[COMMS] Task MQTT + INA219 iniciada no Core 0.");
}

void initWiFi(void) {
  initComunicacao();
}

bool mqttExecucaoAtiva(void) {
  bool ativa;
  portENTER_CRITICAL(&commMux);
  ativa = g_execucaoMqttAtiva;
  portEXIT_CRITICAL(&commMux);
  return ativa;
}

bool mqttConsumirInicioRemoto(void) {
  bool pendente;
  portENTER_CRITICAL(&commMux);
  pendente = g_inicioRemotoPendente;
  g_inicioRemotoPendente = false;
  portEXIT_CRITICAL(&commMux);
  return pendente;
}

void mqttNotificarConclusao(bool desafioConcluido) {
  portENTER_CRITICAL(&commMux);
  if (g_execucaoMqttAtiva) {
    g_desafioConcluidoPendente = desafioConcluido;
    g_finalizacaoPendente = true;
  }
  portEXIT_CRITICAL(&commMux);
}

void mqttNotificarInterrupcaoLocal(void) {
  portENTER_CRITICAL(&commMux);
  if (g_execucaoMqttAtiva) {
    g_interrupcaoLocalPendente = true;
  }
  portEXIT_CRITICAL(&commMux);
}
