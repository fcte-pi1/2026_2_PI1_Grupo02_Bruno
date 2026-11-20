#include "odometria.h"
#include "config_mqtt.h"
#include "controlador.h"

static portMUX_TYPE odometriaMux = portMUX_INITIALIZER_UNLOCKED;

static int g_mazeRows = 4;
static int g_mazeCols = 4;
static StartPosition g_startPos = START_BOTTOM_LEFT;
static int g_row = 3;
static int g_col = 0;
static int g_goalRow = 0;
static int g_goalCol = 3;
static Heading g_heading = HEADING_NORTH;

static float g_distanceMeters = 0.0f;
static float g_speedMetersPerSec = 0.0f;
static bool g_goalReached = false;
static bool g_poseAlterada = false;

// Estado interno do trecho translacional atual
static int32_t g_ultimoDistMovMm = 0;
static int32_t g_proximaCompensacaoRecuoMm = 0;
static int32_t g_compensacaoPendenteMm = 0;
static int32_t g_acumuladoCelulaMm = 0;
static unsigned long g_ultimoTempoVelMs = 0;
static int32_t g_distVelRefMm = 0;

static int clampDim(int valor) {
  if (valor < MAZE_MIN_DIM) return MAZE_MIN_DIM;
  if (valor > MAZE_MAX_DIM) return MAZE_MAX_DIM;
  return valor;
}

static void avancarUmaCelulaLocked(void) {
  int nextRow = g_row;
  int nextCol = g_col;

  switch (g_heading) {
    case HEADING_NORTH: nextRow--; break;
    case HEADING_SOUTH: nextRow++; break;
    case HEADING_EAST:  nextCol++; break;
    case HEADING_WEST:  nextCol--; break;
  }

  if (nextRow < 0) nextRow = 0;
  if (nextRow >= g_mazeRows) nextRow = g_mazeRows - 1;
  if (nextCol < 0) nextCol = 0;
  if (nextCol >= g_mazeCols) nextCol = g_mazeCols - 1;

  if (nextRow != g_row || nextCol != g_col) {
    g_row = nextRow;
    g_col = nextCol;
    g_poseAlterada = true;

    if (g_row == g_goalRow && g_col == g_goalCol) {
      g_goalReached = true;
    }
  }
}

void odometriaConfigurarExecucao(int rows, int cols, StartPosition startPos) {
  int r = clampDim(rows);
  int c = clampDim(cols);

  portENTER_CRITICAL(&odometriaMux);
  g_mazeRows = r;
  g_mazeCols = c;
  g_startPos = startPos;

  // Convenção do grid (idêntica ao backend/frontend):
  // row = 0 é o topo, row = rows - 1 é a base; col = 0 é a esquerda, col = cols - 1 é a direita.
  // Em bottom_right o robô larga apontando para a esquerda (WEST); em bottom_left, para a direita (EAST).
  if (startPos == START_BOTTOM_RIGHT) {
    g_row = r - 1;
    g_col = c - 1;
    g_goalRow = 0;
    g_goalCol = 0;
    g_heading = HEADING_WEST;
  } else {
    g_row = r - 1;
    g_col = 0;
    g_goalRow = 0;
    g_goalCol = c - 1;
    g_heading = HEADING_EAST;
  }

  g_distanceMeters = 0.0f;
  g_speedMetersPerSec = 0.0f;
  g_goalReached = (g_row == g_goalRow && g_col == g_goalCol);
  g_poseAlterada = true;

  g_ultimoDistMovMm = 0;
  g_proximaCompensacaoRecuoMm = 0;
  g_compensacaoPendenteMm = 0;
  g_acumuladoCelulaMm = 0;
  g_ultimoTempoVelMs = millis();
  g_distVelRefMm = 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaDefinirCompensacaoRecuo(int32_t compensacaoRecuoMm) {
  portENTER_CRITICAL(&odometriaMux);
  g_proximaCompensacaoRecuoMm = (compensacaoRecuoMm > 0) ? compensacaoRecuoMm : 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaIniciarMovimentoFrente(void) {
  portENTER_CRITICAL(&odometriaMux);
  g_ultimoDistMovMm = 0;
  g_compensacaoPendenteMm = g_proximaCompensacaoRecuoMm;
  g_proximaCompensacaoRecuoMm = 0;
  g_ultimoTempoVelMs = millis();
  g_distVelRefMm = 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaAtualizarAvanco(int32_t distanciaMovimentoMm) {
  if (distanciaMovimentoMm < 0) return;

  unsigned long agoraMs = millis();

  portENTER_CRITICAL(&odometriaMux);
  int32_t deltaMm = distanciaMovimentoMm - g_ultimoDistMovMm;
  if (deltaMm > 0) {
    g_ultimoDistMovMm = distanciaMovimentoMm;

    // Desconta primeiro eventual recuo de alinhamento para não duplicar distância líquida
    int32_t deltaLiquidoMm = deltaMm;
    if (g_compensacaoPendenteMm > 0) {
      if (deltaLiquidoMm <= g_compensacaoPendenteMm) {
        g_compensacaoPendenteMm -= deltaLiquidoMm;
        deltaLiquidoMm = 0;
      } else {
        deltaLiquidoMm -= g_compensacaoPendenteMm;
        g_compensacaoPendenteMm = 0;
      }
    }

    if (deltaLiquidoMm > 0) {
      g_distanceMeters += ((float)deltaLiquidoMm) / 1000.0f;
      g_acumuladoCelulaMm += deltaLiquidoMm;

      while (g_acumuladoCelulaMm >= CELULA) {
        g_acumuladoCelulaMm -= CELULA;
        avancarUmaCelulaLocked();
        if (g_goalReached) {
          g_acumuladoCelulaMm = 0;
          break;
        }
      }
    }
  }

  unsigned long dtMs = agoraMs - g_ultimoTempoVelMs;
  if (dtMs >= 100) {
    int32_t dDistMm = distanciaMovimentoMm - g_distVelRefMm;
    float velInst = (dDistMm > 0) ? ((float)dDistMm / (float)dtMs) : 0.0f; // mm/ms == m/s
    g_speedMetersPerSec = 0.6f * velInst + 0.4f * g_speedMetersPerSec;
    g_ultimoTempoVelMs = agoraMs;
    g_distVelRefMm = distanciaMovimentoMm;
  }
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaRegistrarParadaMovimento(void) {
  portENTER_CRITICAL(&odometriaMux);
  // Mantém a última velocidade média do trecho em vez de zerar abruptamente entre micro-manobras
  g_speedMetersPerSec = g_speedMetersPerSec * 0.85f;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaAplicarGiro(int16_t graus) {
  portENTER_CRITICAL(&odometriaMux);
  // Se o robô percorreu mais de meia célula (>= 82 mm) antes de virar (ex.: parou no sensor frontal),
  // consolida a entrada na célula antes de rotacionar a orientação.
  if (g_acumuladoCelulaMm >= (CELULA / 2)) {
    avancarUmaCelulaLocked();
  }

  bool giro90 = false;
  if (graus == 180 || graus == -180) {
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 2) % 4);
    // Soma o deslocamento físico da manobra de beco sem saída (avanço pré-180 + arco)
    g_distanceMeters += 0.08f;
  } else if (graus < 0) {
    // Convenção do firmware (ratocego.ino): graus < 0 = curva à direita (horário: N -> E -> S -> W)
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 1) % 4);
    giro90 = true;
    // Arco percorrido pelo centro do robô no giro pivot de 90° (~70 mm)
    g_distanceMeters += 0.07f;
  } else if (graus > 0) {
    // Convenção do firmware (ratocego.ino): graus > 0 = curva à esquerda (anti-horário: N -> W -> S -> E)
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 3) % 4);
    giro90 = true;
    g_distanceMeters += 0.07f;
  }

  if (g_speedMetersPerSec < 0.08f) {
    g_speedMetersPerSec = 0.11f;
  }

  // Em curvas pivot de 90° (1 roda parada), o centro do robô já desloca ~55 mm na nova direção;
  // iniciar o acumulado em 55 mm evita perder a célula seguinte se o sensor frontal parar antes de 165 mm.
  g_acumuladoCelulaMm = giro90 ? 55 : 0;
  g_poseAlterada = true;
  portEXIT_CRITICAL(&odometriaMux);
}

bool odometriaObjetivoAlcancado(void) {
  bool alcancado;
  portENTER_CRITICAL(&odometriaMux);
  alcancado = g_goalReached;
  portEXIT_CRITICAL(&odometriaMux);
  return alcancado;
}

OdometriaSnapshot odometriaObterSnapshot(bool limparFlagAlterada) {
  OdometriaSnapshot snap;
  portENTER_CRITICAL(&odometriaMux);
  snap.mazeRows = g_mazeRows;
  snap.mazeCols = g_mazeCols;
  snap.startPos = g_startPos;
  snap.row = g_row;
  snap.col = g_col;
  snap.goalRow = g_goalRow;
  snap.goalCol = g_goalCol;
  snap.heading = g_heading;
  snap.distanceMeters = g_distanceMeters;
  snap.speedMetersPerSec = g_speedMetersPerSec;
  snap.goalReached = g_goalReached;
  snap.poseAlterada = g_poseAlterada;
  if (limparFlagAlterada) {
    g_poseAlterada = false;
  }
  portEXIT_CRITICAL(&odometriaMux);
  return snap;
}

const char* headingToString(Heading heading) {
  switch (heading) {
    case HEADING_NORTH: return "NORTH";
    case HEADING_EAST:  return "EAST";
    case HEADING_SOUTH: return "SOUTH";
    case HEADING_WEST:  return "WEST";
    default:            return "NORTH";
  }
}

const char* startPositionToString(StartPosition pos) {
  return (pos == START_BOTTOM_RIGHT) ? "bottom_right" : "bottom_left";
}
