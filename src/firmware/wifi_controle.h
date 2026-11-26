#ifndef WIFI_CONTROLE_H
#define WIFI_CONTROLE_H

#include <Arduino.h>

void initWiFi();
void handleWiFi();

void atualizarEstadoWiFi(const String& estado);

void atualizarTelemetriaWiFi(
  int frente,
  int esquerda,
  int direita,
  bool paredeFrente,
  bool paredeEsquerda,
  bool paredeDireita,
  const String& acao
);

extern bool iniciarMissao;
extern bool solicitacaoParadaWiFi;
extern bool ina219Disponivel;

#endif