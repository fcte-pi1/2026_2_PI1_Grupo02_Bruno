#ifndef ODOMETRIA_H_
#define ODOMETRIA_H_

#include <Arduino.h>
#include <stdint.h>

enum Heading : uint8_t {
  HEADING_NORTH = 0,
  HEADING_EAST  = 1,
  HEADING_SOUTH = 2,
  HEADING_WEST  = 3
};

enum StartPosition : uint8_t {
  START_BOTTOM_LEFT  = 0,
  START_BOTTOM_RIGHT = 1
};

struct OdometriaSnapshot {
  int mazeRows;
  int mazeCols;
  StartPosition startPos;
  int row;
  int col;
  int goalRow;
  int goalCol;
  Heading heading;
  float distanceMeters;
  float speedMetersPerSec;
  bool goalReached;
  bool poseAlterada;
};

// Configura uma nova execução: zera odometria, posiciona no canto inicial e define a célula objetivo oposta.
void odometriaConfigurarExecucao(int rows, int cols, StartPosition startPos);

// Define compensação de recuo para o próximo movimento FRENTE (usado após alinharNaParedeTraseira).
void odometriaDefinirCompensacaoRecuo(int32_t compensacaoRecuoMm);

// Chamado no início de cada movimento translacional para frente (FRENTE / FRENTE_ATE_EVENTO).
void odometriaIniciarMovimentoFrente(void);

// Chamado continuamente durante o avanço reto com a distância atual do movimento em mm.
void odometriaAtualizarAvanco(int32_t distanciaMovimentoMm);

// Zera a velocidade instantânea quando o robô para um movimento.
void odometriaRegistrarParadaMovimento(void);

// Atualiza orientação discreta após uma curva completada (-90 = direita, +90 = esquerda, 180 = meia-volta).
void odometriaAplicarGiro(int16_t graus);

// Indica se o robô atingiu a célula diagonalmente oposta ao canto inicial.
bool odometriaObjetivoAlcancado(void);

// Lê um snapshot consistente (thread-safe entre Core 0 e Core 1).
// Se limparFlagAlterada = true, limpa a flag poseAlterada atomicamente.
OdometriaSnapshot odometriaObterSnapshot(bool limparFlagAlterada = false);

const char* headingToString(Heading heading);
const char* startPositionToString(StartPosition pos);

#endif // ODOMETRIA_H_
