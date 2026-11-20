//corpo dos sensores 09/11/2026 - 16h09;

#include "sensores.h"

//variáveis externas;
int16_t FRONTAL = 0;
int16_t ESQUERDO = 0;
int16_t DIREITO = 0;
int16_t TRASEIRO = 0;

/*
FUNÇÃO PARA LEITURA DOS SENSORES DE PAREDE
  ATUALIZA OS SENSORES FRONTAIS E LATERAIS.
  RETORNA UMA MÁSCARA DE BITS INDICANDO A PRESENÇA (1) OU NÃO (0)
  DE PAREDES. O BIT MAIS SIGNIFICATIVO REPRESENTA A PAREDE DA
  ESQUERDA. EX.: 1100 = PRESENÇA DE PAREDE FRONTAL E ESQUERDA;
*/

uint8_t getSensoresParede(void){
  uint8_t paredes = 0;

  FRONTAL = analogRead(FRONTAL_RECEPTOR);
  ESQUERDO = analogRead(ESQUERDO_RECEPTOR);
  DIREITO = analogRead(DIREITO_RECEPTOR);
  TRASEIRO = analogRead(TRASEIRO_RECEPTOR);

  //avaliação de limiar para montar a máscara de bits
  
  if (FRONTAL < FRONTAL_TH){
    paredes |= 0b010; //bit 2: presença de parede na frente;
  }

  if (ESQUERDO < LATERAL_TH){
    paredes |= 0b100; //bit 1: presença de parede à esquerda;
  }

  if (DIREITO < LATERAL_TH){
    paredes |= 0b001; //bit 0: presença de à direita;
  }

  return paredes;

}

//erro de alinhamento para o controle PID;
// Erro de alinhamento para o controle PID:
// Erro > 0: Muito perto da parede DIREITA (precisa puxar para a ESQUERDA)
// Erro < 0: Muito perto da parede ESQUERDA (precisa puxar para a DIREITA)
// Substitua a função getErroSensores() no final de sensores.cpp:
int16_t getErroSensores(void) {
  int16_t erro = 0;

  if (FRONTAL > FRONTAL_TH) {
    int16_t esq_lim = (ESQUERDO < 100) ? ESQUERDO : 100;
    int16_t dir_lim = (DIREITO < 100)  ? DIREITO  : 100;

    // CASO 1: Parede apenas na DIREITA
    if (ESQUERDO >= LATERAL_TH && DIREITO < LATERAL_TH) {
      erro = CENTRO_LATERAL - dir_lim;
    }
    // CASO 2: Parede apenas na ESQUERDA
    else if (DIREITO >= LATERAL_TH && ESQUERDO < LATERAL_TH) {
      erro = esq_lim - CENTRO_LATERAL;
    }
    // CASO 3: Parede nos DOIS LADOS
    else if (ESQUERDO < LATERAL_TH && DIREITO < LATERAL_TH) {
      erro = esq_lim - dir_lim;
    }

    if (abs(erro) < 2) {
      erro = 0;
    }
  }

  return erro;
}
