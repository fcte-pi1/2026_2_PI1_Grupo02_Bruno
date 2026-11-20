#include "seguidor_parede.h"
#include <Ticker.h>

#define PINO_BOOT 0
#define TEMPO_LARGADA_MS 1500 // Tempo [ms] após soltar o BOOT para tirar a mão

volatile bool bRodarControle = false;
volatile bool bAguardandoLargada = false;
volatile bool bParadoPeloBotaoBoot = false;
volatile uint8_t paredes = 0;

Ticker timerControle;

void initEncoders() {
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder_esquerda.attachHalfQuad(C1_ENC_E, C2_ENC_E);
  encoder_direita.attachHalfQuad(C1_ENC_D, C2_ENC_D);
  encoder_esquerda.clearCount();
  encoder_direita.clearCount();
}

void checarBotaoBootNoTimer() {
  static uint8_t contPressionado = 0;
  static bool esperandoSoltar = false;

  if (digitalRead(PINO_BOOT) == LOW) {
    if (!esperandoSoltar) {
      contPressionado++;
      if (contPressionado >= 15) { // ~150ms pressionado
        bRodarControle = !bRodarControle;
        esperandoSoltar = true;
        if (!bRodarControle) {
          bAguardandoLargada = false;
          bParadoPeloBotaoBoot = true;
          pararMotores();
        } else {
          bParadoPeloBotaoBoot = false;
          bAguardandoLargada = true;
        }
      }
    }
  } else {
    contPressionado = 0;
    esperandoSoltar = false;
  }
}

void callbackTimer1(void) {
  checarBotaoBootNoTimer();
  paredes = getSensoresParede(); // Atualiza os sensores a cada 10ms

  if (bRodarControle && !bAguardandoLargada) {
    controleVelocidade();
  } else {
    pararMotores();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("=== RATO CEGO - NAVEGACAO CONTINUA POR SENSORES ===");

  pinMode(PINO_BOOT, INPUT_PULLUP);

  pinMode(FRONTAL_RECEPTOR, INPUT);
  pinMode(TRASEIRO_RECEPTOR, INPUT);
  pinMode(DIREITO_RECEPTOR, INPUT);
  pinMode(ESQUERDO_RECEPTOR, INPUT);

  initMotores();
  initEncoders();
  initComunicacao();

  timerControle.attach_ms(TS, callbackTimer1);

  Serial.println("Pronto! Use a interface MQTT ou aperte o botao BOOT para iniciar/pausar.");
}

void loop() {
  if (!bRodarControle) {
    // Notifica o backend APENAS quando o botão BOOT físico parou uma execução ativa
    if (bParadoPeloBotaoBoot) {
      bParadoPeloBotaoBoot = false;
      if (mqttExecucaoAtiva()) {
        odometriaRegistrarParadaMovimento();
        if (odometriaObjetivoAlcancado()) {
          mqttNotificarConclusao(true);
        } else {
          mqttNotificarInterrupcaoLocal();
        }
      }
    }

    static unsigned long ultimoPrint = 0;
    if (millis() - ultimoPrint > 600) {
      ultimoPrint = millis();
      DadosEletricos elec = obterDadosEletricos();
      Serial.printf("[PAUSADO] Sensores -> F:%d | T:%d | E:%d | D:%d | Paredes(E-F-D): %d%d%d | INA219(%s): %.2fV %.0fmA %.2fW\n",
                    FRONTAL, TRASEIRO, ESQUERDO, DIREITO,
                    (paredes >> 2) & 1, (paredes >> 1) & 1, paredes & 1,
                    elec.disponivel ? "OK" : "OFF",
                    elec.tensaoVolts, elec.correnteMilliAmps, elec.potenciaWatts);
    }
    delay(20);
    return;
  }

  // Novo comando start recebido via MQTT: reseta encoders/controlador imediatamente
  if (mqttConsumirInicioRemoto()) {
    resetControlador();
  }

  // Aguarda soltar o botão BOOT e dá tempo de tirar a mão de cima dos sensores
  if (bAguardandoLargada) {
    pararMotores();

    while (digitalRead(PINO_BOOT) == LOW && bRodarControle) {
      delay(10);
    }

    Serial.printf(">>> Iniciando em %d ms... tire a mao!\n", TEMPO_LARGADA_MS);
    unsigned long inicio = millis();
    while (millis() - inicio < TEMPO_LARGADA_MS) {
      if (!bRodarControle) return;
      delay(10);
    }

    resetControlador();
    OdometriaSnapshot snapAtual = odometriaObterSnapshot(false);
    odometriaConfigurarExecucao(snapAtual.mazeRows, snapAtual.mazeCols, snapAtual.startPos);
    bAguardandoLargada = false;
    delay(30);

    Serial.printf(">>> LARGADA! Sensores -> F:%d | T:%d | E:%d | D:%d | Paredes(E-F-D): %d%d%d\n",
                  FRONTAL, TRASEIRO, ESQUERDO, DIREITO,
                  (paredes >> 2) & 1, (paredes >> 1) & 1, paredes & 1);
  }

  uint8_t estadoParedes = paredes;

  Serial.printf("Paredes (E-F-D): %d%d%d\n",
                (estadoParedes >> 2) & 1, (estadoParedes >> 1) & 1, estadoParedes & 1);

  // MÁQUINA DE ESTADOS - SEGUIDOR DE PAREDE DIREITA CONTÍNUO
  switch (estadoParedes) {
    case 0b001: // Parede na direita (e frente livre)
    case 0b101: // Corredor (paredes nos dois lados e frente livre)
      FRENTE_ATE_EVENTO();
      break;

    case 0b000: // Cruzamento aberto -> Vira à direita SEM ré
    case 0b100: // Sem parede frontal fechando o canto -> Vira à direita SEM ré
    case 0b010: // Entroncamento em T (sem parede atrás após girar) -> Vira à direita SEM ré
      curva(-90, false);
      break;

    case 0b110: // Canto fechado (parede atrás + esquerda garantidas) -> Vira à direita COM ré
      curva(-90, true);
      break;

    case 0b011: // Canto fechado (parede atrás + direita garantidas) -> Vira à esquerda COM ré
      curva(90, true);
      break;

    case 0b111: // Beco sem saída (parede atrás + duas laterais garantidas) -> Avança, gira 180° e alinha
      pararMotores();
      delay(200);
      giro180();
      delay(30);
      FRENTE_ATE_EVENTO();
      break;
  }
}