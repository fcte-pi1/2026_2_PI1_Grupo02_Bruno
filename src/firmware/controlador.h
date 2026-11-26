#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <stdint.h>
#include <math.h>
#include <ESP32Encoder.h>
#include "motores.h"
#include "sensores.h"

// Pinos dos encoders
#define C1_ENC_E 32
#define C2_ENC_E 33
#define C1_ENC_D 25
#define C2_ENC_D 26

// LED azul interno da ESP32 (alerta de colisão/ré)
#define PINO_LED_AZUL 2

// Tipo de movimento
#define PARADO         -1
#define TRANSLACIONAL   0
#define ROTACIONAL      1
#define ALINHAMENTO_RE  2

// Parâmetros físicos do robô
#define ENCODER_PPR      600
#define DIAMETRO_RODA    44
#define RODA_RODA        90
#define CNT_POR_1000MM   4341
#define CNT_POR_360DEG   615
#define CELULA           165 // Tamanho da célula [mm]

// Distância extra [mm] que o robô avança após o sensor direito perder a parede vindo de RETA:
#define AVANCO_POS_BORDA 28

// Detecção de abertura à direita logo após uma CURVA (sem parede na frente):
#define DIST_MIN_PAREDE_SAIDA         5   // Mínimo vendo parede/divisória na direita para armar a borda
#define DIST_MIN_BORDA_POS_CURVA      10  // Ignora oscilações bem no início da saída da curva
#define DIST_SAIR_CURVA_COM_RE        28  // Distância p/ direita aberta (curva COM ré) - era 38
#define DIST_SAIR_CURVA_SEM_RE        14  // Distância p/ direita aberta (curva SEM ré) - era 22
#define AVANCO_POS_BORDA_SAIDA_CURVA  15  // Pós-borda exclusivo quando a parede acaba saindo de curva - era 25

// Distância [mm] que o robô avança para a frente antes de dar o giro de 180°:
#define AVANCO_ANTES_180 30

// Parâmetros da rotação do giro de 180° (duas rodas opostas)
#define GRAUS_GIRO_180       240 // Alvo em graus do giro de 180°
#define PWM_GIRO_180_FRENTE  145 // Força da roda esquerda p/ frente no 180°
#define PWM_GIRO_180_TRAS    185 // Força da roda direita p/ trás no 180°

// Parâmetros de controle (reta normal)
#define TS              10    // Período de amostragem [ms]
#define PWM_BASE_RETA   65    // Velocidade base para andar reto
#define PWM_MIN_RETA    35    // Limite mínimo na reta
#define PWM_MAX_RETA    110   // Limite máximo na reta
#define PWM_BASE_CURVA  65    // Velocidade base durante o giro
#define KP_SYNC         0.4f  // Ganho de alinhamento dos encoders
#define K_SENSORES      0.5f  // Ganho proporcional dos sensores laterais
#define KD_SENSORES     5.0f  // Amortecimento derivativo para evitar oscilação

// Parâmetros de saída de curva (alinhamento mais firme na célula pós-curva)
#define PWM_BASE_POS_CURVA       58   // Um pouco mais lento logo após a curva
#define MAX_CORRECAO_POS_CURVA   22   // Autoridade extra do PID lateral (normal = 15)

// Parâmetros de detecção de batida/travamento e ré de desatolamento
#define TEMPO_TRAVAMENTO_MS  400  // Tempo parado para detectar que bateu [ms]
#define DIST_RE_DESATOLAR    35   // Distância que ele recua após bater [mm]
#define TEMPO_MAX_RE_MS      700  // Limite de tempo da ré caso haja parede atrás [ms]
#define PWM_BASE_RE          85   // Força base da ré ao desatolar
#define PWM_MIN_RE           70   // Limite mínimo na ré
#define PWM_MAX_RE           120  // Limite máximo na ré

// Parâmetros do alinhamento de ré na parede (wall-squaring em cantos fechados)
#define PWM_ALINHAMENTO_RE             80   // Força do recuo (90° e 180°)
#define TEMPO_MAX_ALINHAMENTO_RE_MS    350  // Tempo máximo nas curvas de 90° [ms]
#define MAX_DIST_ALINHAMENTO_RE        60   // Distância máxima nas curvas de 90° [mm]
#define TEMPO_MAX_ALINHAMENTO_180_MS   600  // Tempo máximo no giro de 180° [ms]
#define MAX_DIST_ALINHAMENTO_180       95   // Distância máxima no giro de 180° [mm]
#define MAX_COMPENSACAO_RECUO_MM       45   // Teto de compensação somado à célula seguinte [mm]

// Macros de leitura dos encoders
#define LER_ENCODER_E     (-encoder_esquerda.getCount())
#define LER_ENCODER_D     (-encoder_direita.getCount())
#define RESET_ENCODER_E   encoder_esquerda.clearCount()
#define RESET_ENCODER_D   encoder_direita.clearCount()

#define COUNTS_TO_MM(cnt)  (((cnt) * 1000) / CNT_POR_1000MM)
#define COUNTS_TO_DEG(cnt) (((cnt) * 360) / CNT_POR_360DEG)

// Protótipos
void fazerPipi(void);
void controleVelocidade(void);
void updateEncoders(void);
void controladorPID(void);
bool isFinalMovimento(void);
void resetControlador(void);
void RE(int32_t mm);
void FRENTE(int32_t mm, bool saidaCurva = false, int32_t distSairCelula = 0);
void FRENTE_ATE_EVENTO(void);
int32_t alinharNaParedeTraseira(int32_t maxDistMm = MAX_DIST_ALINHAMENTO_RE,
                                unsigned long maxTempoMs = TEMPO_MAX_ALINHAMENTO_RE_MS);
void curvaPivot(int16_t graus);
void curva(int16_t graus, bool alinharAtras = false);
void giro180(void);

// Variáveis externas
extern int32_t distancia_mm;
extern int32_t distancia_deg;
extern ESP32Encoder encoder_esquerda;
extern ESP32Encoder encoder_direita;
extern volatile bool bRodarControle;
extern volatile uint8_t paredes;

#endif // CONTROLADOR_H_