#ifndef MOTORES_H_
#define MOTORES_H_

#include <Arduino.h>

// DEFINIÇÃO DE PINOS VALIDADA NA SUA PLACA (L298N)
#define PWM_E 23 // Enable do motor esquerdo
#define IN1_E 16 // Canal PWM Esquerdo (LEDC)
#define IN2_E 17 // Direção Esquerda

#define PWM_D 27 // Enable do motor direito
#define IN3_D 19 // Canal PWM Direito (LEDC)
#define IN4_D 18 // Direção Direita

void initMotores(void);
void pararMotores(void);
void setMotores(int16_t pwm_esq, int16_t pwm_dir);

#endif // MOTORES_H_