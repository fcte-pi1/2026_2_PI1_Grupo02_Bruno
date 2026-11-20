# Firmware — Rato Cego (ESP32)

Firmware embarcado do robô **Micromouse ("Rato Cego")** desenvolvido para **ESP32** com arquitetura **FreeRTOS Dual-Core**:
- **Core 1 (`loop()` + `Ticker` de 10 ms)**: Máquina de estados do seguidor de parede direita, controle PID de centralização lateral, leitura dos 4 sensores infravermelhos (ADC1), encoders quadratura via hardware PCNT, acionamento da ponte H L298N (LEDC PWM) e odometria passiva.
- **Core 0 (`taskComunicacaoCore0`)**: Conexão Wi-Fi (`WIFI_STA`), cliente MQTT (`PubSubClient`) para comandos remotos (`start` / `interrupt`) e publicação de telemetria (`4 Hz`), além do monitoramento elétrico via barramento I²C (**INA219**).

---

## 1. Estrutura dos Arquivos

| Arquivo | Responsabilidade |
| :--- | :--- |
| [`ratocego.ino`](ratocego.ino) | Inicialização (`setup()`), timer de controle (`10 ms`), leitura do botão físico `BOOT` e loop principal de navegação. |
| [`controlador.h`](controlador.h) / [`controlador.cpp`](controlador.cpp) | Controle PID lateral, leitura dos encoders, detecção de travamento/colisão, alinhamento de ré e primitivas de movimento (`FRENTE`, `FRENTE_ATE_EVENTO`, `RE`, `curva`, `giro180`). |
| [`seguidor_parede.h`](seguidor_parede.h) | Lógica de decisão do algoritmo seguidor de parede direita e verificação de conclusão do percurso. |
| [`motores.h`](motores.h) / [`motores.cpp`](motores.cpp) | Acionamento PWM dos motores DC N20 pela ponte H L298N. |
| [`sensores.h`](sensores.h) / [`sensores.cpp`](sensores.cpp) | Leitura analógica dos 4 fototransistores IR (`FRONTAL`, `TRASEIRO`, `DIREITO`, `ESQUERDO`) e cálculo do erro lateral. |
| [`odometria.h`](odometria.h) / [`odometria.cpp`](odometria.cpp) | Observador passivo *thread-safe* de posição `(row, column)` em células de `16,5 × 16,5 cm`, orientação (`NORTH`, `EAST`, `SOUTH`, `WEST`), distância percorrida (`m`) e velocidade média (`m/s`). |
| [`wifi_controle.h`](wifi_controle.h) / [`wifi_controle.cpp`](wifi_controle.cpp) | Task FreeRTOS no Core 0 para gerenciamento de Wi-Fi, protocolo MQTT com o backend e leitura do sensor elétrico INA219. |
| [`config_mqtt.h`](config_mqtt.h) | Configurações de rede Wi-Fi, IP/credenciais do broker MQTT, pinos I²C e constantes de temporização. |
| [`upload_esp32.ps1`](upload_esp32.ps1) | Script PowerShell automatizado para compilar e gravar o firmware no ESP32 via `arduino-cli`. |

---

## 2. Pré-requisitos e O Que Instalar

1. **[Arduino IDE 2.x](https://www.arduino.cc/en/software)** instalada no Windows.
2. **Driver USB-Serial CH340 / CP210x** (para que o ESP32 apareça como porta `COM` no Gerenciador de Dispositivos).
3. **Pacote de placas ESP32 versão `3.3.13`** na Arduino IDE:
   - Em *File → Preferences → Additional boards manager URLs*, adicione:
     ```text
     https://espressif.github.io/arduino-esp32/package_esp32_index.json
     ```
   - Em *Tools → Board → Boards Manager*, procure por **`esp32` by Espressif Systems** e instale a versão **`3.3.13`**.
   > **Importante:** Utilize a versão **`3.3.13`** (baseada no ESP-IDF 5). A versão `4.0.0` remove constantes legadas do periférico PCNT utilizadas pela biblioteca `ESP32Encoder`.
4. **Bibliotecas obrigatórias** (instale pelo *Library Manager* da Arduino IDE):
   - **`ESP32Encoder`** (by Kevin Harrington / madhephaestus)
   - **`PubSubClient`** (by Nick O'Leary)
   - **`ArduinoJson`** (by Benoit Blanchon, v7.x)
   - **`Adafruit INA219`** (by Adafruit) + **`Adafruit BusIO`**

---

## 3. Configuração de Rede e MQTT (`config_mqtt.h`)

Antes de gravar o firmware, edite o arquivo [`config_mqtt.h`](config_mqtt.h):

1. Descubra o IPv4 da máquina onde o broker Mosquitto (Docker) está rodando:
   ```powershell
   ipconfig
   ```
2. Atualize as constantes em [`config_mqtt.h`](config_mqtt.h):
   ```cpp
   // Rede Wi-Fi 2.4 GHz (mesma rede local do computador com Docker)
   #define WIFI_SSID "NOME_DO_WIFI"
   #define WIFI_PASSWORD "SENHA_DO_WIFI"

   // IP local do computador onde o container Mosquitto está rodando na porta 1883
   #define MQTT_BROKER_HOST "192.168.15.155"
   #define MQTT_BROKER_PORT 1883
   #define MQTT_USERNAME "ratocego"
   #define MQTT_PASSWORD "teste123" // Deve ser idêntica ao MQTT_PASSWORD de src/backend/.env
   ```

---

## 4. Como Compilar e Gravar no ESP32

### Opção A — Script PowerShell Automatizado (Recomendado no Windows)

O script [`upload_esp32.ps1`](upload_esp32.ps1) mapeia automaticamente uma unidade virtual (`R:`) para contornar o bug do compilador GCC do ESP32 com nomes de usuário do Windows que possuem acento, compila o projeto e detecta a porta `COM` do ESP32:

1. Conecte o ESP32 via USB e **feche o Monitor Serial** da Arduino IDE.
2. No PowerShell, execute a partir da raiz do repositório (ou de `src/firmware`):
   ```powershell
   powershell -ExecutionPolicy Bypass -File src\firmware\upload_esp32.ps1
   ```
   - Para especificar a porta manualmente (ex.: `COM9`):
     ```powershell
     powershell -ExecutionPolicy Bypass -File src\firmware\upload_esp32.ps1 -Port COM9
     ```
   - Para apenas validar a compilação sem gravar:
     ```powershell
     powershell -ExecutionPolicy Bypass -File src\firmware\upload_esp32.ps1 -CompileOnly
     ```

### Opção B — Pela Arduino IDE

1. Abra [`ratocego.ino`](ratocego.ino) na Arduino IDE.
2. Selecione a placa **ESP32 Dev Module** (*Tools → Board → esp32 → ESP32 Dev Module*).
3. Selecione a porta serial correspondente (*Tools → Port*).
4. Clique em **Upload**.

---

## 5. Como Rodar Integrado com o Backend e o Frontend

1. **Suba o Backend, Banco PostgreSQL e Broker Mosquitto** (em [`src/backend`](../backend)):
   - Certifique-se de que `src/backend/.env` contém o mesmo `MQTT_USERNAME` e `MQTT_PASSWORD` definidos em [`config_mqtt.h`](config_mqtt.h).
   - Execute:
     ```powershell
     cd src\backend
     docker compose up -d --build
     ```
2. **Suba o Frontend Web** (em [`src/frontend`](../frontend)):
   ```powershell
   cd src\frontend
   npm install
   npm run dev
   ```
   Acesse `http://localhost:5173`.
3. **Ligue o Robô (ESP32)**:
   - Pelo Monitor Serial (`115200 baud`), confirme as mensagens:
     - `[WIFI] Conectado! IP do ESP32: ...`
     - `[MQTT] Conectado ao broker Mosquitto!`
   - Na interface web (`http://localhost:5173`), escolha as dimensões do labirinto (até `18×18`), a posição de largada (`bottom_left` ou `bottom_right`) e clique em **Iniciar** ou **Interromper**.

---

## 6. Pinagem Utilizada no ESP32

| Subsistema | Sinal | Pino GPIO (ESP32) |
| :--- | :--- | :--- |
| **Sensores IR (ADC1)** | Frontal / Traseiro / Direito / Esquerdo | `GPIO 34` / `GPIO 35` / `GPIO 36` / `GPIO 39` |
| **Motor Esquerdo (L298N)** | `PWM_E` / `IN1_E` / `IN2_E` | `GPIO 23` / `GPIO 16` / `GPIO 17` |
| **Motor Direito (L298N)** | `PWM_D` / `IN3_D` / `IN4_D` | `GPIO 27` / `GPIO 19` / `GPIO 18` |
| **Encoder Esquerdo** | `C1_ENC_E` / `C2_ENC_E` | `GPIO 32` / `GPIO 33` |
| **Encoder Direito** | `C1_ENC_D` / `C2_ENC_D` | `GPIO 25` / `GPIO 26` |
| **Sensor Elétrico INA219 (I²C)** | `SDA` / `SCL` | `GPIO 21` / `GPIO 22` |
| **Interface Local** | Botão `BOOT` / LED Azul | `GPIO 0` / `GPIO 2` |

---

## 7. Alterações Recentes e Pendências Conhecidas

### Alterações aplicadas nesta etapa
- **Calibração da odometria para encoders em `HalfQuad` ([`odometria.cpp`](odometria.cpp)):**
  - Como `initEncoders()` opera em `attachHalfQuad` (`2x`) enquanto `CNT_POR_1000MM = 4341` em `controlador.h` foi dimensionado para `FullQuad` (`4x`), a odometria foi calibrada com `PASSO_CELULA_ENC_MM = 72` (`16,5 cm` físicos por célula) e `ESCALA_DISTANCIA_REAL = 165.0f / 72.0f` para corrigir distância (`m`), velocidade (`m/s`) e avanço de células em linha reta.
- **Saída de curva calibrada para 1 célula real ([`controlador.cpp`](controlador.cpp)):**
  - `AVANCO_SAIDA_CURVA_ENC_MM = 76` em `curva()` (mantendo `usarSensores` ativo via `modoSaidaCurva`), evitando que o robô avance 2 células sem monitorar aberturas laterais após girar 90°.
- **Consolidação imediata ao fim de cada trecho reto (`odometriaConcluirTrechoReto`) e compensação do pivô sem ré ([`odometria.cpp`](odometria.cpp), [`controlador.cpp`](controlador.cpp)):**
  - Consolida a célula final assim que `FRENTE` / `FRENTE_ATE_EVENTO` para os motores (eliminando o atraso de `~2 s` causado por `TEMPO_PAUSA_ANTES_CURVA_MS`).
  - Em curvas de 90° **sem ré** (`curva(±90, false)`), contabiliza `30` unidades de avanço longitudinal ganho pelo pivô de uma roda (`curvaPivot`), zerando esse offset quando há alinhamento de ré (`alinharNaParedeTraseira()`).
  - Limita o desconto de `RE()` em travamentos (`<= 10` unidades) e filtra giros de realinhamento imediatamente após `tratarTravamento()` para não desorientar `g_heading`.
- **Ajuste automático de orientação na largada (`odometriaAjustarOrientacaoLargada`):**
  - Mantém `WEST` em `bottom_right` e `EAST` em `bottom_left`, ajustando para `NORTH` automaticamente caso os sensores laterais detectem o robô apontando para cima na célula inicial.

### Falhas conhecidas e itens pendentes para próximos testes
1. **Validação em pista das curvas sem ré (`curva(±90, false)`):**
   - A compensação de `30` unidades do pivô sem ré e a consolidação imediata em `odometriaConcluirTrechoReto()` já foram compiladas e gravadas no ESP32, mas o teste completo em pista foi interrompido por descarga da bateria na Execução `#75`. **Pendente:** testar com bateria carregada (selecionando `Inferior direito` / `bottom_right` na interface para a largada padrão do labirinto) e confirmar o reconhecimento de todas as células pós-curva.
2. **Sensor elétrico INA219 físico sem resposta no barramento I²C:**
   - O scan I²C em `SDA=21 / SCL=22` (e invertido `22 / 21`) não recebeu ACK do módulo INA219 físico. Enquanto o hardware não responde, o firmware envia uma estimativa dinâmica de bateria LiPo 2S. **Pendente:** revisar soldas, alimentação (`VCC`/`GND`) e trilhas `SDA`/`SCL` do módulo INA219 na placa.
3. **Leitura precoce da parede frontal (`FRONTAL_TH = 2200`) em finais de corredor:**
   - Em alguns cantos após corredores de 3 células, `FRONTAL < 2200` para o robô alguns milímetros antes de o sensor lateral liberar a abertura do canto, fazendo o robô ler `0b111` (beco sem saída) e executar `giro180()` em vez da curva de 90°. **Pendente:** avaliar ajuste fino de `FRONTAL_TH` / `LATERAL_TH` em [`sensores.h`](sensores.h) com a bateria carregada.
4. **Queda de tensão (*brownout*) do ESP32 com bateria fraca:**
   - Quando a bateria descarrega (velocidade caindo de `~0,30 m/s` para `< 0,11 m/s`), o pico de corrente dos motores nas curvas (`PWM = 120`) faz a entrada do regulador LM7805 cair abaixo de `~6,8 V`, derrubando a conexão Wi-Fi/MQTT do ESP32. **Recomendação:** recarregar a bateria antes de novas baterias de testes e considerar capacitor de desacoplamento adicional na linha de `5 V` do ESP32.
