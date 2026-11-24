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

// Inicializa/reseta a odometria para uma nova execução do labirinto (1..18 x 1..18)
void odometriaConfigurarExecucao(int rows, int cols, StartPosition startPos);

// Ajusta automaticamente a orientação inicial na largada caso os sensores IR
// indiquem que o robô foi posicionado apontando para NORTH no canto inicial
void odometriaAjustarOrientacaoLargada(uint8_t paredesIniciais);

// Registra uma compensação de recuo (ex.: alinhamento de ré na parede traseira)
// para que o avanço seguinte não duplique a distância já percorrida
void odometriaDefinirCompensacaoRecuo(int32_t compensacaoRecuoMm);

// Subtrai uma fração limitada do acumulador de célula após manobra de desatolamento (RE)
void odometriaRegistrarRecuo(int32_t recuoMm);

// Notifica início de um novo comando de avanço retilíneo (após resetControlador)
void odometriaIniciarMovimentoFrente(void);

// Atualiza o progresso linear durante FRENTE / FRENTE_ATE_EVENTO a partir de distancia_mm
void odometriaAtualizarAvanco(int32_t distanciaMovimentoMm);

// Notifica parada do movimento translacional (ex.: em travamento)
void odometriaRegistrarParadaMovimento(void);

// Consolida imediatamente a célula final ao término normal de FRENTE / FRENTE_ATE_EVENTO
// (antes da pausa de 1500 ms da curva) caso o acumulado residual seja >= meia célula (35 unid)
void odometriaConcluirTrechoReto(void);

// Atualiza a orientação após curvaPivot(graus) ou giro180()
void odometriaAplicarGiro(int16_t graus);

// Verifica se a célula objetivo do labirinto foi alcançada
bool odometriaObjetivoAlcancado(void);

// Obtém cópia thread-safe do estado atual (e opcionalmente limpa a flag poseAlterada)
OdometriaSnapshot odometriaObterSnapshot(bool limparFlagAlterada = false);

// Utilitários de conversão para string (compatíveis com o backend)
const char* headingToString(Heading heading);
const char* startPositionToString(StartPosition pos);

#endif // ODOMETRIA_H_
