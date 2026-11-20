#include "controlador.h"
#include "odometria.h"

int32_t distancia_mm = 0;
int32_t distancia_deg = 0;

ESP32Encoder encoder_esquerda;
ESP32Encoder encoder_direita;

int8_t tipo_movimento = PARADO;
int32_t setpoint_dist = 0;
int16_t sentido_movimento = 1;
int32_t enc_esq_cnt = 0, enc_dir_cnt = 0;
int32_t offset_enc = 0;
int16_t erro_sensor_ant = 0;
bool modoAteEvento = false;
bool modoSaidaCurva = false;
bool giroDuasRodas = false;
bool ignorarParedeFrente = false;
bool houveTravamento = false;

#define PWM_FORCA_GIRO_FRENTE    120  // Força inicial da curva de 90°
#define PWM_MIN_GIRO_FRENTE      80   // Força final da rampa nos últimos graus do 90°
#define RAMPA_DESACEL_CURVA_DEG  30   // Quantos graus antes do alvo começa a desacelerar
#define KP_GIRO                  1.2f

#define TEMPO_PAUSA_ANTES_CURVA_MS 1500

// Pisca o LED azul da ESP32 para sinalizar batida/travamento
void fazerPipi(void) {
  pinMode(PINO_LED_AZUL, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(PINO_LED_AZUL, HIGH);
    delay(100);
    digitalWrite(PINO_LED_AZUL, LOW);
    delay(80);
  }
}

void tratarTravamento(void) {
  houveTravamento = true;
  tipo_movimento = PARADO;
  pararMotores();
  delay(50);

  Serial.println(">>> [BATEU/TRAVOU] Piscando LED azul e recuando...");
  fazerPipi();

  if (bRodarControle) {
    RE(DIST_RE_DESATOLAR);
  }
}

void controleVelocidade(void) {
  updateEncoders();
  controladorPID();
}

void updateEncoders(void) {
  enc_esq_cnt = LER_ENCODER_E;
  enc_dir_cnt = LER_ENCODER_D;

  distancia_mm  = COUNTS_TO_MM((abs(enc_dir_cnt) + abs(enc_esq_cnt)) / 2);
  distancia_deg = COUNTS_TO_DEG((abs(enc_dir_cnt) + abs(enc_esq_cnt)) / 2);
}

void controladorPID(void) {
  if (tipo_movimento == PARADO) {
    pararMotores();
    return;
  }

  // ==================== MOVIMENTO EM LINHA RETA (FRENTE OU RÉ DE DESATOLAMENTO) ====================
  if (tipo_movimento == TRANSLACIONAL) {
    if (sentido_movimento > 0) {
      bool usarSensores = modoAteEvento || (setpoint_dist > 80);
      int16_t erro_bruto_sensor = usarSensores ? getErroSensores() : 0;

      int correcao_sensor = 0;
      int correcao_enc = 0;
      int limite_correcao = modoSaidaCurva ? MAX_CORRECAO_POS_CURVA : 15;
      int pwm_base        = modoSaidaCurva ? PWM_BASE_POS_CURVA : PWM_BASE_RETA;

      if (erro_bruto_sensor != 0) {
        int16_t delta_erro = erro_bruto_sensor - erro_sensor_ant;

        if (abs(delta_erro) > 30) {
          delta_erro = 0;
        }

        float termo_P = (float)erro_bruto_sensor / K_SENSORES;
        float termo_D = KD_SENSORES * (float)delta_erro;

        correcao_sensor = constrain((int)(termo_P + termo_D), -limite_correcao, limite_correcao);
        offset_enc = abs(enc_esq_cnt) - abs(enc_dir_cnt);
      } else {
        int32_t erro_alinhamento = (abs(enc_esq_cnt) - abs(enc_dir_cnt)) - offset_enc;
        correcao_enc = (int)(KP_SYNC * erro_alinhamento);
      }

      erro_sensor_ant = erro_bruto_sensor;

      int pwm_esq = constrain(pwm_base + correcao_enc + correcao_sensor, PWM_MIN_RETA, PWM_MAX_RETA);
      int pwm_dir = constrain(pwm_base - correcao_enc - correcao_sensor, PWM_MIN_RETA, PWM_MAX_RETA);
      setMotores(pwm_esq, pwm_dir);
    }
    else {
      // Ré sincronizada pelos encoders para recuar reto ao bater
      int32_t erro_alinhamento = abs(enc_esq_cnt) - abs(enc_dir_cnt);
      int correcao_enc = constrain((int)(KP_SYNC * erro_alinhamento), -20, 20);

      int pwm_esq = constrain(PWM_BASE_RE - correcao_enc, PWM_MIN_RE, PWM_MAX_RE);
      int pwm_dir = constrain(PWM_BASE_RE + correcao_enc, PWM_MIN_RE, PWM_MAX_RE);
      setMotores(-pwm_esq, -pwm_dir);
    }
  }
  // ==================== CURVAS (90 COM RAMPA OU 180 COM 2 RODAS OPOSTAS) ====================
  else if (tipo_movimento == ROTACIONAL) {
    if (!giroDuasRodas) {
      // Curva de 90° com rampa de desaceleração nos últimos graus para evitar passar do ponto
      int32_t falta_deg = setpoint_dist - distancia_deg;
      int pwm_giro = PWM_FORCA_GIRO_FRENTE;

      if (falta_deg < RAMPA_DESACEL_CURVA_DEG) {
        pwm_giro = map(constrain(falta_deg, 0, RAMPA_DESACEL_CURVA_DEG),
                       0, RAMPA_DESACEL_CURVA_DEG,
                       PWM_MIN_GIRO_FRENTE, PWM_FORCA_GIRO_FRENTE);
      }

      if (sentido_movimento < 0) {
        setMotores(pwm_giro, 0);
      } else {
        setMotores(0, pwm_giro);
      }
    }
    else {
      // Giro de 180° no próprio eixo: roda esquerda p/ frente e roda direita p/ trás
      int32_t erro_giro = abs(enc_esq_cnt) - abs(enc_dir_cnt);
      int correcao_giro = constrain((int)(KP_SYNC * erro_giro), -25, 25);

      int pwm_esq_frente = constrain(PWM_GIRO_180_FRENTE - correcao_giro, 65, 240);
      int pwm_dir_tras   = constrain(PWM_GIRO_180_TRAS   + correcao_giro, 80, 240);

      setMotores(pwm_esq_frente, -pwm_dir_tras);
    }
  }
  // ==================== ALINHAMENTO DE RÉ NA PAREDE TRASEIRA ====================
  else if (tipo_movimento == ALINHAMENTO_RE) {
    // Malha aberta (sem KP_SYNC): o lado que tocar primeiro para enquanto o outro alinha o chassi
    setMotores(-PWM_ALINHAMENTO_RE, -PWM_ALINHAMENTO_RE);
  }
}

bool isFinalMovimento(void) {
  if (!bRodarControle) return true;

  if (tipo_movimento == TRANSLACIONAL) {
    bool temParedeFrente  = (paredes & 0b010) != 0;
    bool semParedeDireita = (paredes & 0b001) == 0;

    if (!ignorarParedeFrente && sentido_movimento > 0 && temParedeFrente) {
      return true;
    }

    // Limite de segurança durante o avanço pré-180 para não encostar na parede frontal
    if (ignorarParedeFrente && sentido_movimento > 0 && FRONTAL <= CENTRO_FRONTAL) {
      return true;
    }

    if (modoAteEvento && sentido_movimento > 0) {
      if (semParedeDireita) {
        return true;
      }
    } else {
      if (distancia_mm >= setpoint_dist) {
        return true;
      }
    }
  }
  else if (tipo_movimento == ROTACIONAL) {
    if (distancia_deg >= setpoint_dist) {
      return true;
    }
  }
  else if (tipo_movimento == ALINHAMENTO_RE) {
    if (distancia_mm >= setpoint_dist) {
      return true;
    }
  }

  return false;
}

void resetControlador(void) {
  RESET_ENCODER_E;
  RESET_ENCODER_D;
  enc_esq_cnt = 0;
  enc_dir_cnt = 0;
  offset_enc = 0;
  erro_sensor_ant = 0;
  distancia_mm = 0;
  distancia_deg = 0;
}

void RE(int32_t mm) {
  if (!bRodarControle || mm == 0) return;

  resetControlador();
  modoAteEvento = false;
  modoSaidaCurva = false;
  giroDuasRodas = false;
  ignorarParedeFrente = true;
  setpoint_dist = abs(mm);
  sentido_movimento = -1;
  tipo_movimento = TRANSLACIONAL;

  unsigned long inicio = millis();
  while (!isFinalMovimento()) {
    if (!bRodarControle || (millis() - inicio >= TEMPO_MAX_RE_MS)) {
      break;
    }
    delay(2);
  }

  tipo_movimento = PARADO;
  ignorarParedeFrente = false;
  pararMotores();
  delay(120);
}

void FRENTE(int32_t mm, bool saidaCurva) {
  if (!bRodarControle || mm == 0) return;

  houveTravamento = false;
  resetControlador();
  odometriaIniciarMovimentoFrente();
  modoAteEvento = false;
  modoSaidaCurva = saidaCurva;
  giroDuasRodas = false;
  ignorarParedeFrente = false;
  setpoint_dist = abs(mm);
  sentido_movimento = 1;
  tipo_movimento = TRANSLACIONAL;

  unsigned long tempoUltimoMovimento = millis();
  int32_t ultimaDistancia = distancia_mm;

  while (!isFinalMovimento()) {
    odometriaAtualizarAvanco(distancia_mm);
    if (abs(distancia_mm - ultimaDistancia) >= 2) {
      ultimaDistancia = distancia_mm;
      tempoUltimoMovimento = millis();
    } else if (millis() - tempoUltimoMovimento >= TEMPO_TRAVAMENTO_MS) {
      modoSaidaCurva = false;
      odometriaRegistrarParadaMovimento();
      tratarTravamento();
      return;
    }
    delay(2);
  }

  odometriaAtualizarAvanco(distancia_mm);
  tipo_movimento = PARADO;
  modoSaidaCurva = false;
  pararMotores();
  odometriaRegistrarParadaMovimento();
  delay(150);
}

void FRENTE_ATE_EVENTO(void) {
  if (!bRodarControle) return;

  houveTravamento = false;
  resetControlador();
  odometriaIniciarMovimentoFrente();
  modoAteEvento = true;
  modoSaidaCurva = false;
  giroDuasRodas = false;
  ignorarParedeFrente = false;
  sentido_movimento = 1;
  tipo_movimento = TRANSLACIONAL;

  unsigned long tempoUltimoMovimento = millis();
  int32_t ultimaDistancia = distancia_mm;

  while (!isFinalMovimento()) {
    odometriaAtualizarAvanco(distancia_mm);
    if (abs(distancia_mm - ultimaDistancia) >= 2) {
      ultimaDistancia = distancia_mm;
      tempoUltimoMovimento = millis();
    } else if (millis() - tempoUltimoMovimento >= TEMPO_TRAVAMENTO_MS) {
      odometriaRegistrarParadaMovimento();
      tratarTravamento();
      return;
    }
    delay(2);
  }

  odometriaAtualizarAvanco(distancia_mm);
  tipo_movimento = PARADO;
  pararMotores();
  odometriaRegistrarParadaMovimento();

  bool temParedeFrente  = (paredes & 0b010) != 0;
  bool semParedeDireita = (paredes & 0b001) == 0;

  if (semParedeDireita && !temParedeFrente && bRodarControle) {
    FRENTE(AVANCO_POS_BORDA, false);
  } else {
    delay(150);
  }
}

int32_t alinharNaParedeTraseira(int32_t maxDistMm, unsigned long maxTempoMs) {
  if (!bRodarControle) return 0;

  tipo_movimento = PARADO;
  pararMotores();
  delay(80);

  Serial.printf(">>> [ALINHAMENTO] Encostando na parede (max %d mm / %lu ms)...\n",
                (int)maxDistMm, maxTempoMs);

  resetControlador();
  modoAteEvento = false;
  modoSaidaCurva = false;
  giroDuasRodas = false;
  ignorarParedeFrente = true;
  setpoint_dist = maxDistMm;
  tipo_movimento = ALINHAMENTO_RE;

  unsigned long inicio = millis();
  unsigned long ultimoPulso = inicio;
  int32_t ultimoEncE = 0;
  int32_t ultimoEncD = 0;

  while (!isFinalMovimento()) {
    if (!bRodarControle) break;

    if (abs(enc_esq_cnt - ultimoEncE) > 2 || abs(enc_dir_cnt - ultimoEncD) > 2) {
      ultimoEncE = enc_esq_cnt;
      ultimoEncD = enc_dir_cnt;
      ultimoPulso = millis();
    }

    unsigned long decorrido = millis() - inicio;
    if ((decorrido >= 150 && (millis() - ultimoPulso) >= 100) ||
        (decorrido >= maxTempoMs)) {
      break;
    }

    delay(5);
  }

  int32_t recuo_mm = constrain(distancia_mm, 0, MAX_COMPENSACAO_RECUO_MM);

  tipo_movimento = PARADO;
  ignorarParedeFrente = false;
  pararMotores();
  delay(120);

  Serial.printf(">>> [ALINHAMENTO] Concluido! Recuo medido: %d mm (compensado: %d mm)\n",
                (int)distancia_mm, (int)recuo_mm);
  return recuo_mm;
}

void curvaPivot(int16_t graus) {
  if (!bRodarControle || graus == 0) return;

  houveTravamento = false;
  tipo_movimento = PARADO;
  pararMotores();

  Serial.printf(">>> [PAUSA VISUAL] Parado por %d ms antes de girar %d graus (Paredes E-F-D: %d%d%d)...\n",
                TEMPO_PAUSA_ANTES_CURVA_MS, graus,
                (paredes >> 2) & 1, (paredes >> 1) & 1, paredes & 1);

  unsigned long inicioPausa = millis();
  while (millis() - inicioPausa < TEMPO_PAUSA_ANTES_CURVA_MS) {
    if (!bRodarControle) return;
    delay(10);
  }

  resetControlador();
  modoAteEvento = false;
  modoSaidaCurva = false;
  giroDuasRodas = false;
  ignorarParedeFrente = false;
  setpoint_dist = abs(graus);
  sentido_movimento = (graus > 0) ? -1 : 1;
  tipo_movimento = ROTACIONAL;

  unsigned long tempoUltimoMovimento = millis();
  int32_t ultimoGrau = distancia_deg;

  while (!isFinalMovimento()) {
    if (abs(distancia_deg - ultimoGrau) >= 3) {
      ultimoGrau = distancia_deg;
      tempoUltimoMovimento = millis();
    } else if (millis() - tempoUltimoMovimento >= TEMPO_TRAVAMENTO_MS) {
      tratarTravamento();
      return;
    }
    delay(2);
  }

  tipo_movimento = PARADO;
  pararMotores();
  if (!houveTravamento && bRodarControle) {
    odometriaAplicarGiro(graus);
  }
  delay(200);
}

void curva(int16_t graus, bool alinharAtras) {
  curvaPivot(graus);
  if (houveTravamento || !bRodarControle) return;

  int32_t recuo = 0;
  if (alinharAtras && bRodarControle) {
    recuo = alinharNaParedeTraseira();
  }

  odometriaDefinirCompensacaoRecuo(recuo);
  FRENTE(CELULA + recuo, true);
}

void giro180(void) {
  if (!bRodarControle) return;

  houveTravamento = false;
  tipo_movimento = PARADO;
  pararMotores();

  // 1. Avança um pouco para a frente antes de girar 180 graus
  if (AVANCO_ANTES_180 > 0 && FRONTAL > CENTRO_FRONTAL) {
    Serial.printf(">>> [BECO SEM SAIDA] Avancando %d mm antes do giro de 180...\n", AVANCO_ANTES_180);

    resetControlador();
    modoAteEvento = false;
    modoSaidaCurva = false;
    giroDuasRodas = false;
    ignorarParedeFrente = true;
    setpoint_dist = AVANCO_ANTES_180;
    sentido_movimento = 1;
    tipo_movimento = TRANSLACIONAL;

    unsigned long tempoUltimoMovimento = millis();
    int32_t ultimaDistancia = distancia_mm;

    while (!isFinalMovimento()) {
      if (abs(distancia_mm - ultimaDistancia) >= 2) {
        ultimaDistancia = distancia_mm;
        tempoUltimoMovimento = millis();
      } else if (millis() - tempoUltimoMovimento >= TEMPO_TRAVAMENTO_MS) {
        ignorarParedeFrente = false;
        tratarTravamento();
        return;
      }
      delay(2);
    }

    ignorarParedeFrente = false;
    tipo_movimento = PARADO;
    pararMotores();
    delay(150);
  }

  // 2. Executa o giro de 180 graus com as duas rodas
  Serial.println(">>> [BECO SEM SAIDA] Iniciando giro de 180 graus com as duas rodas...");

  resetControlador();
  modoAteEvento = false;
  modoSaidaCurva = false;
  giroDuasRodas = true;
  ignorarParedeFrente = false;
  setpoint_dist = GRAUS_GIRO_180;
  tipo_movimento = ROTACIONAL;

  unsigned long tempoUltimoMovimento = millis();
  int32_t ultimoGrau = distancia_deg;

  while (!isFinalMovimento()) {
    if (abs(distancia_deg - ultimoGrau) >= 3) {
      ultimoGrau = distancia_deg;
      tempoUltimoMovimento = millis();
    } else if (millis() - tempoUltimoMovimento >= TEMPO_TRAVAMENTO_MS) {
      giroDuasRodas = false;
      tratarTravamento();
      return;
    }
    delay(2);
  }

  tipo_movimento = PARADO;
  giroDuasRodas = false;
  pararMotores();
  if (!houveTravamento && bRodarControle) {
    odometriaAplicarGiro(180);
  }
  delay(100);

  // 3. No beco sem saída há parede atrás e nos dois lados: alinha com curso maior antes de sair
  if (bRodarControle) {
    alinharNaParedeTraseira(MAX_DIST_ALINHAMENTO_180, TEMPO_MAX_ALINHAMENTO_180_MS);
  }
}