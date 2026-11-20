#include "motores.h"

void initMotores(void) {
  pinMode(PWM_E, OUTPUT);
  pinMode(IN1_E, OUTPUT);
  pinMode(IN2_E, OUTPUT);

  pinMode(PWM_D, OUTPUT);
  pinMode(IN3_D, OUTPUT);
  pinMode(IN4_D, OUTPUT);

  pararMotores();
}

void pararMotores(void) {
  // Desliga os Enables (23 e 27) para cortar a força imediatamente
  digitalWrite(PWM_E, LOW);
  digitalWrite(PWM_D, LOW);

  // Coloca direção em LOW e PWM em 0 (usando APENAS analogWrite em IN1_E e IN3_D!)
  digitalWrite(IN2_E, LOW);
  analogWrite(IN1_E, 0);

  digitalWrite(IN4_D, LOW);
  analogWrite(IN3_D, 0);
}

void setMotores(int16_t pwm_esq, int16_t pwm_dir) {
  if (pwm_esq == 0 && pwm_dir == 0) {
    pararMotores();
    return;
  }

  // ---------------- MOTOR ESQUERDO ----------------
  if (pwm_esq > 0) {
    // Frente (Modo 1)
    digitalWrite(PWM_E, HIGH);
    digitalWrite(IN2_E, HIGH);
    analogWrite(IN1_E, 255 - constrain(pwm_esq, 0, 250));
  } 
  else if (pwm_esq < 0) {
    // Ré
    digitalWrite(PWM_E, HIGH);
    digitalWrite(IN2_E, LOW);
    analogWrite(IN1_E, constrain(-pwm_esq, 0, 250));
  } 
  else {
    // Roda Esquerda PARADA (serve de apoio na curva de 1 roda)
    digitalWrite(PWM_E, HIGH); // Mantém Enable ativo com IN1=0 e IN2=0 para frear a roda!
    digitalWrite(IN2_E, LOW);
    analogWrite(IN1_E, 0);
  }

  // ---------------- MOTOR DIREITO ----------------
  if (pwm_dir > 0) {
    // Frente (Modo 1)
    digitalWrite(PWM_D, HIGH);
    digitalWrite(IN4_D, HIGH);
    analogWrite(IN3_D, 255 - constrain(pwm_dir, 0, 250));
  } 
  else if (pwm_dir < 0) {
    // Ré
    digitalWrite(PWM_D, HIGH);
    digitalWrite(IN4_D, LOW);
    analogWrite(IN3_D, constrain(-pwm_dir, 0, 250));
  } 
  else {
    // Roda Direita PARADA (serve de apoio na curva de 1 roda)
    digitalWrite(PWM_D, HIGH); // Mantém Enable ativo com IN3=0 e IN4=0 para frear a roda!
    digitalWrite(IN4_D, LOW);
    analogWrite(IN3_D, 0);
  }
}