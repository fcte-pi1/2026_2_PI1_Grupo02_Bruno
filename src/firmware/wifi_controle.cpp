#include "wifi_controle.h"

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// ---------------- CONFIGURAÇÕES WI-FI ----------------
const char* WIFI_NOME  = "rato_Cego";
const char* WIFI_SENHA = "12345678";

// ---------------- OBJETOS ----------------
WebServer server(80);
Adafruit_INA219 ina219;

// ---------------- VARIÁVEIS ----------------
bool iniciarMissao = false;
bool solicitacaoParadaWiFi = false;
bool ina219Disponivel = false;

String estadoRobo = "Parado";
String ultimaAcaoWiFi = "Parado";

int rawF = 0;
int rawE = 0;
int rawD = 0;

bool paredeF = false;
bool paredeE = false;
bool paredeD = false;

float tensaoBateria = 0.0;
float correnteBateria = 0.0;

unsigned long ultimaLeituraINA = 0;

// ---------------- INTERFACE WEB ----------------
const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>MicroMouse</title>
<style>
  body { font-family: Arial, sans-serif; background-color: #222; color: #fff; text-align: center; max-width: 400px; margin: auto; padding: 20px; }
  h2, h3 { color: #ffeb3b; }
  button { padding: 15px 30px; font-size: 20px; font-weight: bold; border: none; border-radius: 8px; cursor: pointer; margin: 10px; width: 40%; }
  .btn-iniciar { background-color: #4CAF50; color: white; }
  .btn-parar { background-color: #f44336; color: white; }
  .card { background-color: #333; border-radius: 8px; padding: 15px; margin: 10px 0; text-align: left; }
</style>
</head>
<body>
<h2>MICROMOUSE</h2>
<button class="btn-iniciar" onclick="comando('/start')">INICIAR</button>
<button class="btn-parar" onclick="comando('/stop')">PARAR</button>
<div class="card">
  <p>Estado: <strong id="estado" style="color:#03a9f4">Conectando...</strong></p>
  <p>Última ação: <strong id="acao">Parado</strong></p>
</div>
<div class="card">
  <h3>⚡ Bateria (INA219)</h3>
  <p>Tensão: <strong id="tensao">0.00 V</strong></p>
  <p>Corrente: <strong id="corrente">0.00 mA</strong></p>
  <p>Status: <strong id="ina">Verificando...</strong></p>
</div>
<div class="card">
  <h3>📏 Sensores</h3>
  <p>Frente: <strong id="frente">0</strong> <span id="pf"></span></p>
  <p>Esquerda: <strong id="esquerda">0</strong> <span id="pe"></span></p>
  <p>Direita: <strong id="direita">0</strong> <span id="pd"></span></p>
</div>
<script>
async function comando(endereco) {
  try {
    const resposta = await fetch(endereco, { method: 'GET', cache: 'no-store' });
    if (!resposta.ok) throw new Error("HTTP " + resposta.status);
    atualizarDados();
  } catch (erro) {
    document.getElementById("estado").textContent = "Erro de envio";
  }
}
let atualizando = false;
async function atualizarDados() {
  if (atualizando) return;
  atualizando = true;
  try {
    const resposta = await fetch('/dados', { method: 'GET', cache: 'no-store' });
    if (!resposta.ok) throw new Error("Erro HTTP");
    const j = await resposta.json();
    document.getElementById("estado").textContent = j.estado;
    document.getElementById("tensao").textContent = Number(j.v).toFixed(2) + " V";
    document.getElementById("corrente").textContent = Number(j.i).toFixed(0) + " mA";
    document.getElementById("ina").textContent = j.ina ? "Ativo" : "Desconectado";
    document.getElementById("frente").textContent = j.f;
    document.getElementById("esquerda").textContent = j.e;
    document.getElementById("direita").textContent = j.d;
    document.getElementById("pf").textContent = j.pf ? " (Parede)" : " (Livre)";
    document.getElementById("pe").textContent = j.pe ? " (Parede)" : " (Livre)";
    document.getElementById("pd").textContent = j.pd ? " (Parede)" : " (Livre)";
    document.getElementById("acao").textContent = j.acao;
  } catch (erro) {
    document.getElementById("estado").textContent = "Sem conexão";
  }
  atualizando = false;
}
setInterval(atualizarDados, 500);
atualizarDados();
</script>
</body>
</html>
)rawliteral";

// ---------------- LEITURA DO INA219 ----------------
void lerINA219() {
  if (!ina219Disponivel) {
    tensaoBateria = 0;
    correnteBateria = 0;
    return;
  }
  unsigned long agora = millis();
  if (agora - ultimaLeituraINA >= 500) {
    ultimaLeituraINA = agora;
    
    // A biblioteca da Adafruit tem um método que já soma a tensão do Shunt
    tensaoBateria = ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000.0);
    correnteBateria = ina219.getCurrent_mA();
  }
}

// ---------------- ENVIO DOS DADOS ----------------
void enviarDados() {
  String json = "{";
  json += "\"estado\":\"" + estadoRobo + "\"";
  json += ",\"f\":" + String(rawF);
  json += ",\"e\":" + String(rawE);
  json += ",\"d\":" + String(rawD);
  json += ",\"pf\":" + String(paredeF ? "true" : "false");
  json += ",\"pe\":" + String(paredeE ? "true" : "false");
  json += ",\"pd\":" + String(paredeD ? "true" : "false");
  json += ",\"acao\":\"" + ultimaAcaoWiFi + "\"";
  json += ",\"v\":" + String(tensaoBateria, 2);
  json += ",\"i\":" + String(correnteBateria, 0);
  json += ",\"ina\":" + String(ina219Disponivel ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

// ---------------- COMANDO INICIAR ----------------
void acionarPartida() {
  solicitacaoParadaWiFi = false;
  iniciarMissao = true;
  estadoRobo = "A Andar!";
  ultimaAcaoWiFi = "Partida acionada";
  Serial.println("[WIFI] Partida solicitada!");
  server.send(200, "text/plain", "ok");
}

// ---------------- COMANDO PARAR ----------------
void acionarParada() {
  iniciarMissao = false; 
  solicitacaoParadaWiFi = true; // ATENÇÃO: O ficheiro principal TEM de ler isto!
  estadoRobo = "Parado";
  ultimaAcaoWiFi = "Parada solicitada";
  Serial.println("[WIFI] Parada solicitada!");
  server.send(200, "text/plain", "ok");
}

void atualizarEstadoWiFi(const String& estado) { estadoRobo = estado; }

void atualizarTelemetriaWiFi(int frente, int esquerda, int direita, bool pFrente, bool pEsquerda, bool pDireita, const String& acao) {
  rawF = frente; rawE = esquerda; rawD = direita;
  paredeF = pFrente; paredeE = pEsquerda; paredeD = pDireita;
  ultimaAcaoWiFi = acao;
}

// ---------------- INICIALIZAÇÃO ----------------
void initWiFi() {
  Wire.begin(21, 22);
  
  // Limpa configurações de rede presas
  WiFi.disconnect(true);
  delay(100);

  // Inicializa INA219
  ina219Disponivel = ina219.begin();
  if (ina219Disponivel) {
    ina219.setCalibration_32V_2A(); // Calibração padrão forçada
    Serial.println("[INA219] Conectado!");
  } else {
    Serial.println("[INA219] Nao encontrado!");
  }

  // Inicializa AP
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(WIFI_NOME, WIFI_SENHA)) {
    Serial.println("[WIFI] Falha ao iniciar AP!");
    return;
  }

  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.softAPIP());

  // Rotas
  server.on("/", []() { server.send_P(200, "text/html", PAGINA); });
  server.on("/dados", enviarDados);
  server.on("/start", acionarPartida);
  server.on("/stop", acionarParada);
  server.onNotFound([]() { server.send(404, "text/plain", "Nao encontrado"); });

  server.begin();
  Serial.println("[WIFI] Servidor iniciado!");
}

void handleWiFi() {
  server.handleClient();
  lerINA219();
}