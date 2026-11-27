#ifndef CONFIG_MQTT_H_
#define CONFIG_MQTT_H_

// =============================================================================
// CONFIGURAÇÕES DE WI-FI E MQTT DO RATO CEGO (ESP32)
// Ajuste SSID, senha e o IP local da máquina onde o Docker (Mosquitto) está
// rodando.
// =============================================================================

// Rede Wi-Fi (mesma rede local do PC onde o Docker/Mosquitto está rodando)
#define WIFI_SSID "OLIVEIRA"
#define WIFI_PASSWORD "edu591730"

// Broker MQTT (IP da máquina com Docker na rede local, ex.: "192.168.1.100")
#define MQTT_BROKER_HOST "192.168.15.155"
#define MQTT_BROKER_PORT 1883
#define MQTT_USERNAME "ratocego"
#define MQTT_PASSWORD "ratocego"
#define MQTT_CLIENT_ID "ratocego-esp32"
#define MQTT_TOPIC_PREFIX "ratocego"

// Parâmetros de temporização e protocolo
#define MQTT_KEEP_ALIVE_SEC 10       // Alinhado ao RNF33 do backend (10 s)
#define MQTT_PACKET_BUFFER_SIZE 1024 // Necessário para JSONs de telemetria
#define WIFI_RETRY_INTERVAL_MS 5000
#define MQTT_RETRY_INTERVAL_MS 3000
#define TELEMETRY_INTERVAL_MS 250   // Publicação periódica em execução (4 Hz)
#define INA219_READ_INTERVAL_MS 200 // Taxa de amostragem do sensor I2C (5 Hz)

// Pinos I2C do sensor INA219 (livres de conflito com motores, encoders e IR)
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// Limites de dimensões do labirinto (especificação: até 18x18 células de 16,5
// cm)
#define MAZE_MIN_DIM 1
#define MAZE_MAX_DIM 18

// Sincronização NTP (fallback caso o robô seja iniciado pelo botão BOOT físico)
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.nist.gov"

#endif // CONFIG_MQTT_H_
