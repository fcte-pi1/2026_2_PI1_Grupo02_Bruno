//cabeçalho seguidor_parede 09/11/2026 - 17h01;

#ifndef seguidor_parede_H_
#define seguidor_parede_H_

#include <Arduino.h>
#include "motores.h"
#include "sensores.h"
#include "controlador.h"
#include "odometria.h"
#include "wifi_controle.h"
#include <stdint.h>
                                                                              // UTILIZAR SOMENTE CASO SEJA NECESSÁRIO INVERTER QUAL PAREDE SEGUIR;
//constantes para definir qual parede o robô vai seguir;
//#define ESQUERDA 1
//#define DIREITA 2

//protótipo das funções;
void setup(void);
void loop(void);
void callbackTimer1(void); //PARA O PID NÃO ATRASAR POR FICAR NO MEIO DE UMA LÓGICA COMPLEXA NO LOOP,
                           //FOI CRIADA ESSA FUNÇÃO PARA AGIR COMO TEMPORIZADOR PARA O MOTOR E O SENSOR
                           //NUNCA ATRASAREM.

#endif
