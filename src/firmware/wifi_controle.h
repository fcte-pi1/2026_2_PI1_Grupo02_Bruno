#ifndef WIFI_CONTROLE_H_
#define WIFI_CONTROLE_H_

#include <Arduino.h>
#include <stdint.h>

struct DadosEletricos {
  bool disponivel;
  float tensaoVolts;
  float correnteMilliAmps;
  float potenciaWatts;
};

// Inicializa I2C + INA219, conecta ao Wi-Fi Station, configura MQTT e inicia a task de comunicação no Core 0.
void initComunicacao(void);

// Mantido como alias de compatibilidade
void initWiFi(void);

// Consulta se existe uma execução iniciada via MQTT em andamento.
bool mqttExecucaoAtiva(void);

// Consome a flag indicando que um novo comando start MQTT acabou de iniciar a execução (para resetar controlador).
bool mqttConsumirInicioRemoto(void);

// Notifica o backend que o robô concluiu o labirinto (chegou à célula diagonalmente oposta ou falhou).
void mqttNotificarConclusao(bool desafioConcluido);

// Notifica o backend que a execução ativa foi interrompida (ex.: botão físico BOOT pressionado durante run MQTT).
void mqttNotificarInterrupcaoLocal(void);

// Retorna a última leitura elétrica do sensor INA219 (thread-safe).
DadosEletricos obterDadosEletricos(void);

extern volatile bool ina219Disponivel;

#endif // WIFI_CONTROLE_H_
