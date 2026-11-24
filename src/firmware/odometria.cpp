#include "odometria.h"
#include "config_mqtt.h"
#include "controlador.h"

extern volatile uint8_t paredes;

static portMUX_TYPE odometriaMux = portMUX_INITIALIZER_UNLOCKED;

// =============================================================================
// CALIBRAÇÃO FÍSICA DOS ENCODERS (HalfQuad vs FullQuad)
// Em ratocego.ino os encoders operam em attachHalfQuad (2x), enquanto
// CNT_POR_1000MM = 4341 em controlador.h foi dimensionado para FullQuad (4x).
// Na prática (validado nos logs de telemetria em corredor reto de 3 células),
// cada célula física de 165 mm (16,5 cm) gera ~72 unidades em distancia_mm.
// =============================================================================
static const int32_t PASSO_CELULA_ENC_MM = 72;
static const int32_t LIMIAR_CONSOLIDAR_CELULA_GIRO_MM = 36; // Meia célula física (8,25 cm)
static const float   ESCALA_DISTANCIA_REAL = 165.0f / 72.0f; // Converte distancia_mm -> mm reais

static int g_mazeRows = 4;
static int g_mazeCols = 4;
static StartPosition g_startPos = START_BOTTOM_LEFT;
static int g_row = 3;
static int g_col = 0;
static int g_prevRow = 3;
static int g_prevCol = 0;
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
static int32_t g_avancoDesdeUltimoGiroMm = 0;
static bool g_houveRecuoDesatolamento = false;
static unsigned long g_ultimoTempoVelMs = 0;
static int32_t g_distVelRefMm = 0;

static int clampDim(int valor) {
  if (valor < MAZE_MIN_DIM) return MAZE_MIN_DIM;
  if (valor > MAZE_MAX_DIM) return MAZE_MAX_DIM;
  return valor;
}

static void calcularVizinho(int row, int col, Heading h, int& outRow, int& outCol) {
  outRow = row;
  outCol = col;
  switch (h) {
    case HEADING_NORTH: outRow--; break;
    case HEADING_SOUTH: outRow++; break;
    case HEADING_EAST:  outCol++; break;
    case HEADING_WEST:  outCol--; break;
  }
}

static bool passoDentroDoGrid(int row, int col, Heading h) {
  int nr = row, nc = col;
  calcularVizinho(row, col, h, nr, nc);
  return (nr >= 0 && nr < g_mazeRows && nc >= 0 && nc < g_mazeCols);
}

static void avancarUmaCelulaLocked(void) {
  if (!passoDentroDoGrid(g_row, g_col, g_heading)) {
    g_acumuladoCelulaMm = 0;
    return;
  }

  int nextRow = g_row;
  int nextCol = g_col;
  calcularVizinho(g_row, g_col, g_heading, nextRow, nextCol);

  if (nextRow != g_row || nextCol != g_col) {
    g_prevRow = g_row;
    g_prevCol = g_col;
    g_row = nextRow;
    g_col = nextCol;
    g_poseAlterada = true;

    if (g_row == g_goalRow && g_col == g_goalCol) {
      g_goalReached = true;
    }
  }
}

static void aplicarRotacaoHeadingLocked(int16_t graus) {
  if (graus == 180 || graus == -180) {
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 2) % 4);
  } else if (graus < 0) {
    // Convenção do firmware (ratocego.ino): graus < 0 = curva à direita (horário: N -> E -> S -> W)
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 1) % 4);
  } else if (graus > 0) {
    // Convenção do firmware (ratocego.ino): graus > 0 = curva à esquerda (anti-horário: N -> W -> S -> E)
    g_heading = static_cast<Heading>((static_cast<uint8_t>(g_heading) + 3) % 4);
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
  g_prevRow = g_row;
  g_prevCol = g_col;

  g_distanceMeters = 0.0f;
  g_speedMetersPerSec = 0.0f;
  g_goalReached = (g_row == g_goalRow && g_col == g_goalCol);
  g_poseAlterada = true;

  g_ultimoDistMovMm = 0;
  g_proximaCompensacaoRecuoMm = 0;
  g_compensacaoPendenteMm = 0;
  g_acumuladoCelulaMm = 0;
  g_avancoDesdeUltimoGiroMm = PASSO_CELULA_ENC_MM;
  g_houveRecuoDesatolamento = false;
  g_ultimoTempoVelMs = millis();
  g_distVelRefMm = 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaAjustarOrientacaoLargada(uint8_t paredesIniciais) {
  bool temEsq    = (paredesIniciais & 0b100) != 0;
  bool temFrente = (paredesIniciais & 0b010) != 0;
  bool temDir    = (paredesIniciais & 0b001) != 0;

  portENTER_CRITICAL(&odometriaMux);
  if (!temFrente) {
    if (g_startPos == START_BOTTOM_RIGHT) {
      if (!temEsq && temDir) {
        g_heading = HEADING_NORTH;
        g_poseAlterada = true;
      }
    } else {
      if (!temDir && temEsq) {
        g_heading = HEADING_NORTH;
        g_poseAlterada = true;
      }
    }
  }
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaDefinirCompensacaoRecuo(int32_t compensacaoRecuoMm) {
  (void)compensacaoRecuoMm;
  portENTER_CRITICAL(&odometriaMux);
  // Quando há alinhamento de ré na parede traseira após a curva, o recuo desfaz
  // o avanço longitudinal ganho pelo pivô de uma roda (curvaPivot), portanto zera o acumulado.
  g_acumuladoCelulaMm = 0;
  g_proximaCompensacaoRecuoMm = 0;
  g_compensacaoPendenteMm = 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaRegistrarRecuo(int32_t recuoMm) {
  if (recuoMm <= 0) return;
  portENTER_CRITICAL(&odometriaMux);
  // Desconta no máximo 10 unidades por recuo de desatolamento para não zerar o avanço
  // acumulado caso o robô raspe na parede lateral ao longo de um corredor comprido.
  int32_t desconto = (recuoMm > 10) ? 10 : recuoMm;
  if (g_acumuladoCelulaMm > desconto) {
    g_acumuladoCelulaMm -= desconto;
  } else {
    g_acumuladoCelulaMm = 0;
  }
  g_houveRecuoDesatolamento = true;
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

    // Desconta primeiro eventual recuo de alinhamento na parede traseira
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
      // Converte unidades do encoder (HalfQuad) para metros reais percorridos (1 célula = 72 unid = 0,165 m)
      float deltaRealMm = (float)deltaLiquidoMm * ESCALA_DISTANCIA_REAL;
      g_distanceMeters += deltaRealMm / 1000.0f;

      g_acumuladoCelulaMm += deltaLiquidoMm;
      g_avancoDesdeUltimoGiroMm += deltaLiquidoMm;

      if (g_avancoDesdeUltimoGiroMm >= LIMIAR_CONSOLIDAR_CELULA_GIRO_MM) {
        g_houveRecuoDesatolamento = false;
      }

      // Avança 1 célula a cada PASSO_CELULA_ENC_MM (72 unidades = 16,5 cm físicos)
      while (g_acumuladoCelulaMm >= PASSO_CELULA_ENC_MM) {
        g_acumuladoCelulaMm -= PASSO_CELULA_ENC_MM;
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
    float velInst = (dDistMm > 0) ? (((float)dDistMm * ESCALA_DISTANCIA_REAL) / (float)dtMs) : 0.0f;
    g_speedMetersPerSec = 0.6f * velInst + 0.4f * g_speedMetersPerSec;
    g_ultimoTempoVelMs = agoraMs;
    g_distVelRefMm = distanciaMovimentoMm;
  }
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaRegistrarParadaMovimento(void) {
  portENTER_CRITICAL(&odometriaMux);
  g_speedMetersPerSec = g_speedMetersPerSec * 0.85f;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaConcluirTrechoReto(void) {
  portENTER_CRITICAL(&odometriaMux);
  g_speedMetersPerSec = g_speedMetersPerSec * 0.85f;
  // Quando o robô termina normalmente um movimento para frente (parando no sensor frontal,
  // na borda lateral ou no alvo de 1 célula pós-curva), se já percorreu >= 35 unidades (~8 cm)
  // dentro da célula à frente, consolida a entrada imediatamente (sem esperar os 2 s da curva).
  if (g_acumuladoCelulaMm >= 35) {
    avancarUmaCelulaLocked();
  }
  g_acumuladoCelulaMm = 0;
  portEXIT_CRITICAL(&odometriaMux);
}

void odometriaAplicarGiro(int16_t graus) {
  portENTER_CRITICAL(&odometriaMux);
  // Se o robô percorreu pelo menos meia célula física (>= 36 unidades = 8,25 cm) antes da curva
  // (ex.: sensor frontal parou o robô antes de completar as 72 unidades da célula),
  // consolida a entrada nessa célula antes de virar.
  if (g_acumuladoCelulaMm >= LIMIAR_CONSOLIDAR_CELULA_GIRO_MM) {
    avancarUmaCelulaLocked();
  }

  // Se o giro ocorreu imediatamente após uma batida/desatolamento (RE) com avanço < 36 unidades,
  // trata-se apenas de realinhamento do chassi para desencostar da parede lateral do corredor,
  // portanto não rotaciona a bússola de 90° em falso (a menos que seja um giro explícito de 180°).
  bool giro180Explicito = (graus == 180 || graus == -180);
  if (!giro180Explicito && g_houveRecuoDesatolamento && g_avancoDesdeUltimoGiroMm < LIMIAR_CONSOLIDAR_CELULA_GIRO_MM) {
    g_houveRecuoDesatolamento = false;
    g_acumuladoCelulaMm = 0;
    portEXIT_CRITICAL(&odometriaMux);
    return;
  }

  aplicarRotacaoHeadingLocked(graus);

  if (g_speedMetersPerSec < 0.10f) {
    g_speedMetersPerSec = 0.14f;
  }

  // Em curvaPivot de 90° (1 roda parada e 1 roda avançando), o centro do robô já se desloca
  // ~7 cm (~30 unidades de encoder) para frente na nova direção durante o próprio giro.
  // Se NÃO houver ré (alinharAtras == false), esses 30 permanecem acumulados para que mesmo um
  // avanço curto (>= 5 unidades) até a parede frontal da nova célula consolide a célula (>= 35),
  // enquanto o avanço máximo de 76 unidades soma 30 + 76 = 106 (1 célula de 72 + sobra 34 < 35).
  // Se HOUVER ré (alinharAtras == true), odometriaDefinirCompensacaoRecuo zera esse valor.
  g_acumuladoCelulaMm = giro180Explicito ? 0 : 30;
  g_avancoDesdeUltimoGiroMm = 0;
  g_houveRecuoDesatolamento = false;
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
