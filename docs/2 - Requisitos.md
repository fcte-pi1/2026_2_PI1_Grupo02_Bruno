# Requisitos do Projeto

Este documento apresenta os requisitos funcionais (RF) e não funcionais (RNF) do projeto **Rato Cego**, organizados de acordo com os épicos definidos para o desenvolvimento do Micromouse.

Os requisitos servem como base para o planejamento, desenvolvimento, integração e validação do sistema.

---

## Épicos do Projeto

1. **ÉPICO 1 — Estrutura do Micromouse**
2. **ÉPICO 2 — Hardware e Sensoriamento**
3. **ÉPICO 3 — Alimentação e Energia**
4. **ÉPICO 4 — Navegação e Controle**
5. **ÉPICO 5 — Sistema Web e Telemetria**
6. **ÉPICO 6 — Banco de Dados e Histórico**
7. **ÉPICO 7 — Integração e Validação**

---

# Visão Geral

Os requisitos são divididos em:

* **Requisitos Funcionais (RF):** descrevem as funções, comportamentos e serviços que o sistema deve executar.
* **Requisitos Não Funcionais (RNF):** descrevem características de qualidade, restrições, limites técnicos e condições que devem ser atendidas pelo sistema.

## Classificação e Priorização

A prioridade dos requisitos utiliza o modelo **MoSCoW**:

* **Must:** requisito indispensável para o funcionamento do sistema.
* **Should:** requisito importante, mas que pode ser implementado posteriormente caso haja restrições.
* **Could:** requisito desejável, desde que sua implementação não prejudique requisitos de maior prioridade.

---

# Requisitos Funcionais

## ÉPICO 1 — Estrutura do Micromouse

| ID      | Requisito                                     | Descrição                                                                                                                                                | Prioridade |
| ------- | --------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RF1** | Proteção dos componentes internos             | A estrutura deve proteger os componentes eletrônicos, sensores, motores e demais módulos contra choques, colisões e impactos durante a operação.         | Must       |
| **RF2** | Acesso aos componentes internos               | A estrutura deve permitir acesso aos componentes para inspeção, manutenção, substituição e ajustes sem exigir a desmontagem completa do chassi.          | Should     |
| **RF3** | Fixação dos subsistemas                       | A estrutura deve possuir pontos de fixação adequados para os subsistemas de hardware, alimentação e controle, evitando deslocamentos durante a operação. | Must       |
| **RF4** | Modularidade estrutural                       | A estrutura deve permitir a substituição ou atualização de módulos sem exigir a reconstrução completa do chassi.                                         | Should     |
| **RF5** | Fixação e alinhamento do sistema de locomoção | Os motores, rodas e demais elementos de tração devem ser fixados de forma a manter seu posicionamento e alinhamento durante a operação.                  | Must       |
| **RF6** | Acomodação e posicionamento dos sensores      | A estrutura deve possuir espaços e pontos de fixação adequados para os sensores, mantendo sua orientação e evitando obstruções.                          | Must       |
| **RF7** | Organização e proteção do cabeamento          | Os cabos devem ser organizados e protegidos de forma a evitar interferência com rodas, motores, sensores e demais componentes móveis.                    | Must       |
| **RF8** | Acesso aos elementos de operação              | Os elementos externos de operação, como interruptores, conectores e interfaces, devem permanecer acessíveis durante a utilização do Micromouse.          | Should     |

---

## ÉPICO 2 — Hardware e Sensoriamento

| ID       | Requisito                                        | Descrição                                                                                                                                                                                                      | Prioridade |
| -------- | ------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RF9**  | Reconhecimento Espacial e Detecção de Obstáculos | O Micromouse deve realizar amostragem contínua do ambiente por meio dos sensores de proximidade, identificando paredes de aproximadamente 5 cm de altura e aberturas nas direções frontal, lateral e diagonal. | Must       |
| **RF10** | Processamento Embarcado Autônomo                 | O microcontrolador deve executar localmente a leitura dos sensores, a lógica de controle e o armazenamento das informações necessárias à navegação, sem depender de processamento externo.                     | Must       |
| **RF11** | Acionamento e Modulação de Potência Motriz       | O sistema deve converter os comandos lógicos em acionamento elétrico reversível dos motores de tração, permitindo controle contínuo da velocidade por PWM.                                                     | Must       |
| **RF12** | Manutenção de Trajetória Centralizada            | O sistema deve ajustar diferencialmente o acionamento dos motores para manter o Micromouse em trajetória reta e centralizada entre as paredes do labirinto.                                                    | Must       |
| **RF13** | Execução de Curvas e Manobras de Rotação         | O sistema deve permitir a execução de curvas de 90° e 180° por meio do acionamento diferencial dos motores, respeitando as dimensões das células de 18 × 18 cm.                                                | Must       |
| **RF14** | Transmissão Sem Fio de Telemetria                | O Micromouse deve possuir um meio de comunicação sem fio capaz de transmitir os dados necessários à telemetria durante a execução.                                                                             | Must       |
| **RF15** | Regulação e Distribuição de Energia              | O sistema deve receber energia da fonte recarregável e distribuir tensões reguladas e estáveis aos circuitos de controle, sensores e atuadores.                                                                | Must       |
| **RF16** | Interface Física de Operação e Disparo           | O Micromouse deve possuir uma interface física externa que permita iniciar e interromper manualmente a execução.                                                                                               | Must       |

---

## ÉPICO 3 — Alimentação e Energia

| ID       | Requisito                           | Descrição                                                                                                                                                             | Prioridade |
| -------- | ----------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RF17** | Telemetria energética em tempo real | Durante cada execução, o sistema deve coletar e disponibilizar dados sobre o consumo e a condição da bateria, associando essas informações à execução correspondente. | Must       |
| **RF18** | Isolamento manual da alimentação    | O circuito de alimentação deve possuir um interruptor físico acessível capaz de desconectar a bateria dos subsistemas durante transporte, montagem ou manutenção.     | Must       |
| **RF19** | Aviso web de bateria baixa          | O firmware deve monitorar periodicamente a tensão da bateria e, ao atingir o nível crítico definido, enviar um aviso à interface web durante a execução.              | Must       |
| **RF20** | Bateria removível e reinstalável    | A bateria deve poder ser removida e reinstalada sem exigir a desmontagem completa do chassi.                                                                          | Could      |

---

## ÉPICO 4 — Navegação e Controle

| ID       | Requisito                           | Descrição                                                                                                                                                                      | Prioridade |
| -------- | ----------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| **RF21** | Identificação de paredes            | O software deve identificar a presença de paredes a partir dos dados obtidos pelos sensores.                                                                                   | Must       |
| **RF22** | Localização no labirinto            | O sistema deve acompanhar a localização do Micromouse dentro do labirinto durante a execução.                                                                                  | Must       |
| **RF23** | Determinação do percurso            | O sistema deve determinar autonomamente os movimentos necessários com base nas informações obtidas durante a execução.                                                         | Must       |
| **RF24** | Navegação autônoma                  | O Micromouse deve realizar a navegação no labirinto sem intervenção humana durante a execução.                                                                                 | Must       |
| **RF25** | Identificação do objetivo           | O sistema deve identificar quando o Micromouse alcançar a região definida como objetivo do labirinto.                                                                          | Must       |
| **RF26** | Registro do trajeto                 | O sistema deve registrar o trajeto percorrido pelo Micromouse durante cada execução.                                                                                           | Must       |
| **RF27** | Seleção do tipo de labirinto        | O sistema deve permitir ao operador selecionar o tipo de labirinto antes do início da execução.                                                                                | Must       |
| **RF28** | Preparação da execução              | O sistema deve verificar as condições necessárias para iniciar uma execução, incluindo a seleção do labirinto e a disponibilidade dos recursos necessários.                    | Must       |
| **RF29** | Início do percurso                  | O sistema deve permitir ao operador iniciar o percurso após a preparação da execução.                                                                                          | Must       |
| **RF30** | Interrupção do percurso             | O sistema deve permitir ao operador interromper uma execução em andamento por meio da interface disponível.                                                                    | Must       |
| **RF31** | Reinício de execução                | O sistema deve permitir iniciar uma nova execução após a conclusão ou interrupção de uma execução anterior.                                                                    | Should     |
| **RF32** | Gerenciamento do estado da execução | O sistema deve controlar e disponibilizar os estados da execução, incluindo, no mínimo, **aguardando, em execução, concluída, interrompida e encerrada por falha ou timeout**. | Must       |
| **RF33** | Encerramento automático da execução | O sistema deve encerrar automaticamente a execução quando o objetivo for alcançado, o tempo máximo for excedido ou ocorrer uma falha que impeça a continuidade segura.         | Must       |
| **RF34** | Parada segura                       | Quando uma execução for interrompida ou ocorrer uma condição de falha que exija parada, o sistema deve interromper ou limitar o acionamento dos motores de forma segura.       | Must       |
| **RF35** | Medição de deslocamento             | O sistema deve utilizar os encoders dos motores para obter informações de deslocamento e velocidade do Micromouse.                                                             | Must       |

---

## ÉPICO 5 — Sistema Web e Telemetria

| ID       | Requisito                                 | Descrição                                                                                                                                                          | Prioridade |
| -------- | ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| **RF36** | Recepção e disponibilização da telemetria | O sistema web deve receber os dados de telemetria enviados pelo Micromouse e disponibilizá-los para acompanhamento da execução.                                    | Must       |
| **RF37** | Visualização do trajeto                   | A interface web deve apresentar e atualizar o trajeto percorrido pelo Micromouse durante a execução.                                                               | Must       |
| **RF38** | Monitoramento da bateria                  | A interface web deve apresentar a condição e o consumo da bateria durante a execução, utilizando os dados fornecidos pela telemetria energética.                   | Must       |
| **RF39** | Monitoramento do tempo                    | O sistema deve contabilizar o tempo de execução e apresentá-lo na interface web durante o percurso.                                                                | Must       |
| **RF40** | Exibição da velocidade média              | A interface web deve calcular e apresentar a velocidade média da execução.                                                                                         | Must       |
| **RF41** | Resultado do desafio                      | A interface web deve informar se o Micromouse concluiu ou não o desafio ao final da execução.                                                                      | Must       |
| **RF42** | Estabelecimento da conexão WebSocket      | O sistema deve estabelecer uma conexão WebSocket entre a aplicação web e o serviço responsável pela comunicação em tempo real.                                     | Must       |
| **RF43** | Detecção de perda de conexão WebSocket    | O sistema deve detectar a perda ou indisponibilidade da conexão WebSocket durante uma execução.                                                                    | Must       |
| **RF44** | Reconexão WebSocket                       | O sistema deve realizar tentativas automáticas de reconexão WebSocket após uma perda de conexão, respeitando a política de tentativas definida pelo projeto.       | Should     |
| **RF45** | Sinalização de indisponibilidade          | A interface web deve informar ao operador quando uma comunicação necessária para o monitoramento estiver indisponível.                                             | Must       |
| **RF46** | Comunicação MQTT                          | O sistema deve utilizar MQTT para transmissão e recepção das mensagens definidas pela arquitetura de comunicação do projeto.                                       | Must       |
| **RF47** | Reconexão MQTT                            | O sistema deve realizar tentativas de reconexão MQTT após uma perda de conexão.                                                                                    | Must       |
| **RF48** | Tratamento de mensagens MQTT              | O sistema deve receber, validar e processar as mensagens MQTT de acordo com o formato definido para a comunicação, descartando ou sinalizando mensagens inválidas. | Must       |

---

## ÉPICO 6 — Banco de Dados e Histórico

| ID       | Requisito                      | Descrição                                                                                                                                                                    | Prioridade |
| -------- | ------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RF49** | Armazenamento da execução      | Após cada execução, o sistema deve armazenar os dados coletados durante o percurso no banco de dados.                                                                        | Must       |
| **RF50** | Associação ao labirinto        | Os dados armazenados devem ser associados ao tipo de labirinto correspondente à execução.                                                                                    | Must       |
| **RF51** | Consulta por labirinto         | A interface web deve permitir consultar os dados das execuções realizadas em um determinado labirinto.                                                                       | Must       |
| **RF52** | Consulta geral                 | A interface web deve permitir consultar conjuntamente os dados armazenados de diferentes labirintos e execuções.                                                             | Must       |
| **RF53** | Registro de falhas da execução | O sistema deve registrar eventos relevantes que impeçam ou interrompam uma execução, incluindo falhas de comunicação, timeout, interrupções e condições de falha detectadas. | Must       |

---

## ÉPICO 7 — Integração e Validação

| ID       | Requisito                       | Descrição                                                                                                                                                   | Prioridade |
| -------- | ------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RF54** | Correção da movimentação        | O sistema deve utilizar as informações de deslocamento obtidas pelos encoders para auxiliar no controle e posicionamento do Micromouse durante a navegação. | Must       |
| **RF55** | Calibração dos sensores         | O sistema deve permitir a calibração dos sensores antes da operação, utilizando procedimentos definidos pelo projeto.                                       | Should     |
| **RF56** | Sinalização do ciclo de recarga | Durante o processo de recarga, o Micromouse poderá indicar visualmente o estado do ciclo de carregamento.                                                   | Could      |

---

# Requisitos Não Funcionais

Os requisitos não funcionais estabelecem **limites, métricas e características de qualidade** que devem ser atendidos pelo sistema.

Sempre que possível, os requisitos são definidos por meio de métricas mensuráveis, permitindo sua verificação durante os testes.

---

## ÉPICO 1 — Estrutura do Micromouse

| ID        | Requisito                                     | Descrição                                                                                                                                                          | Prioridade |
| --------- | --------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| **RNF1**  | Limite dimensional do chassi                  | O Micromouse deve possuir dimensões máximas de **16,5 cm × 16,5 cm**, considerando qualquer estado operacional permitido pelo projeto.                             | Must       |
| **RNF2**  | Compatibilidade com as dimensões do labirinto | A geometria do Micromouse deve permitir movimentação livre nas células de **18 cm**, sem pontos salientes ou interferências que possam causar travamento ou danos. | Must       |
| **RNF3**  | Massa estrutural                              | A massa da estrutura deve permanecer dentro do limite definido pelo projeto, sem comprometer aceleração, frenagem, estabilidade ou autonomia.                      | Must       |
| **RNF4**  | Resistência mecânica                          | A estrutura deve suportar movimentação, aceleração, frenagem e impactos previstos durante os testes sem sofrer deformações que comprometam seu funcionamento.      | Must       |
| **RNF5**  | Rigidez estrutural                            | A estrutura deve manter sua geometria e o posicionamento dos componentes durante a operação, evitando deformações ou folgas que prejudiquem a navegação.           | Must       |
| **RNF6**  | Estabilidade estrutural                       | O Micromouse deve permanecer estável durante acelerações, frenagens, curvas e mudanças de direção.                                                                 | Must       |
| **RNF7**  | Distribuição de massa                         | A distribuição de massa deve evitar desequilíbrios que prejudiquem a locomoção, estabilidade ou precisão das manobras.                                             | Should     |
| **RNF8**  | Compatibilidade entre subsistemas             | As dimensões, espaços e pontos de fixação da estrutura devem ser compatíveis com os componentes de hardware, alimentação, sensores e demais subsistemas.           | Must       |
| **RNF9**  | Precisão dimensional de fabricação            | As dimensões finais da estrutura devem permanecer dentro das tolerâncias de fabricação definidas no projeto.                                                       | Must       |
| **RNF10** | Segurança estrutural                          | A estrutura não deve apresentar pontas, arestas ou elementos expostos que possam causar danos aos componentes ou comprometer a operação.                           | Must       |
| **RNF11** | Durabilidade da estrutura                     | A estrutura deve manter suas características mecânicas e dimensões dentro das tolerâncias estabelecidas após os testes e execuções previstos.                      | Should     |
| **RNF12** | Facilidade de montagem e desmontagem          | A estrutura deve permitir montagem, desmontagem e manutenção utilizando as ferramentas disponíveis, sem procedimentos excessivamente complexos.                    | Should     |
| **RNF13** | Aproveitamento do espaço interno              | O espaço interno deve ser utilizado de forma eficiente, sem comprometer circulação, manutenção, ventilação ou integração dos subsistemas.                          | Should     |
| **RNF14** | Compatibilidade com os materiais disponíveis  | Os materiais utilizados devem ser compatíveis com os processos de fabricação, ferramentas, orçamento e recursos disponíveis no projeto.                            | Must       |

---

## ÉPICO 2 — Hardware e Sensoriamento

| ID        | Requisito                               | Descrição                                                                                                                                                                          | Prioridade |
| --------- | --------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RNF15** | Proteção e margem de segurança elétrica | O sistema deve possuir proteção contra inversão de polaridade, e os condutores devem suportar pelo menos **30% acima da corrente máxima prevista** para o circuito correspondente. | Must       |
| **RNF16** | Estabilidade de tensão lógica           | Os reguladores devem manter as linhas de **3,3 V e 5 V dentro de ±5%** de seus valores nominais durante condições de operação, incluindo a partida dos motores.                    | Must       |
| **RNF17** | Latência da malha física de resposta    | O intervalo entre a leitura de um sensor e o acionamento efetivo correspondente dos motores deve ser de, no máximo, **50 ms**.                                                     | Must       |

---

## ÉPICO 3 — Alimentação e Energia

| ID        | Requisito                                 | Descrição                                                                                                                                                          | Prioridade |
| --------- | ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| **RNF18** | Autonomia operacional                     | Com sensores, controle, motores e telemetria ativos, o Micromouse deve operar por pelo menos **30 minutos sem recarga ou substituição da bateria**.                | Must       |
| **RNF19** | Rendimento da conversão de energia        | Os conversores e reguladores devem apresentar eficiência mínima de **85% em carga nominal**.                                                                       | Should     |
| **RNF20** | Consumo em inatividade                    | Com o sistema energizado, mas sem executar um percurso, o consumo total deve ser de no máximo **50 mA**, considerando a desativação de periféricos desnecessários. | Should     |
| **RNF21** | Aviso preventivo de descarga              | O sistema deve emitir o aviso de bateria baixa quando qualquer célula atingir **3,3 V**, antes de atingir uma faixa considerada prejudicial à bateria.             | Must       |
| **RNF22** | Restrição de massa do conjunto energético | Os componentes destinados à alimentação devem representar no máximo **25% da massa final do Micromouse**.                                                          | Must       |
| **RNF23** | Integridade da alimentação dos sensores   | Durante acelerações e partidas dos motores, a alimentação dos sensores deve permanecer entre **95% e 105% da tensão nominal**.                                     | Must       |
| **RNF24** | Proteção contra falhas elétricas          | O sistema de alimentação deve interromper ou limitar a corrente em condições de sobrecorrente ou curto-circuito.                                                   | Must       |
| **RNF25** | Confiabilidade da telemetria energética   | Após a calibração, as leituras de tensão e consumo devem apresentar erro máximo de **5% em relação ao instrumento de referência**.                                 | Should     |
| **RNF26** | Compatibilidade de tensão dos subsistemas | A tensão nominal e a faixa de descarga da bateria devem ser compatíveis com todos os subsistemas, e os reguladores devem suportar as tensões e correntes exigidas. | Must       |

---

## ÉPICO 4 — Navegação e Controle

| ID        | Requisito                              | Descrição                                                                                                                                                                                                                                         | Prioridade |
| --------- | -------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RNF27** | Tempo máximo de execução               | Cada tentativa de resolução do labirinto deve ser encerrada em no máximo **10 minutos**.                                                                                                                                                          | Must       |
| **RNF28** | Compatibilidade com os labirintos      | O software de navegação deve ser compatível com os labirintos de **4 × 4, 8 × 4 e 12 × 4 células**, considerando células de **18 cm**.                                                                                                            | Must       |
| **RNF29** | Compatibilidade com o hardware         | O software embarcado deve ser compatível com os componentes eletrônicos utilizados para sensoriamento, navegação e comunicação.                                                                                                                   | Must       |
| **RNF30** | Compatibilidade com o microcontrolador | O software embarcado deve ser executável no **ESP32** definido para o projeto.                                                                                                                                                                    | Must       |
| **RNF31** | Timeout da conexão WebSocket           | O sistema deve detectar a ausência de comunicação válida no WebSocket após, no máximo, **X segundos** sem recebimento de mensagem ou heartbeat.                                                                                                   | Must       |
| **RNF32** | Tempo de reconexão WebSocket           | Após a perda de conexão, o sistema deve realizar novas tentativas de conexão a cada **X segundos**, durante no máximo **Y tentativas**, conforme os valores definidos pela equipe.                                                                | Should     |
| **RNF33** | Timeout e Keep-Alive MQTT              | A comunicação MQTT deve utilizar mecanismo de keep-alive com intervalo máximo de **X segundos**, considerando a conexão perdida após **Y segundos** sem comunicação válida.                                                                       | Must       |
| **RNF34** | Reconexão MQTT                         | Após a perda da conexão MQTT, o sistema deve realizar tentativas automáticas de reconexão em intervalos de **X segundos**, durante no máximo **Y tentativas**.                                                                                    | Must       |
| **RNF35** | Integridade das mensagens              | As mensagens trocadas pelo sistema devem seguir o formato definido pela arquitetura de comunicação. Mensagens inválidas, incompletas ou incompatíveis devem ser identificadas e descartadas ou sinalizadas sem provocar comportamento inesperado. | Must       |
| **RNF36** | Latência da telemetria                 | Os dados de telemetria devem estar disponíveis para a interface web em até **X ms** após sua geração no sistema embarcado, durante uma execução normal.                                                                                           | Must       |
| **RNF37** | Precisão de sensoriamento              | Os sensores utilizados para navegação devem apresentar precisão suficiente para identificar paredes, obstáculos e aberturas dentro das tolerâncias estabelecidas nos testes do projeto.                                                           | Should     |

---

## ÉPICO 5 — Sistema Web e Telemetria

| ID        | Requisito                               | Descrição                                                                                                                                                                                             | Prioridade |
| --------- | --------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RNF38** | Atualização da telemetria               | A interface web deve atualizar os dados de telemetria continuamente durante uma execução, respeitando a frequência definida pela arquitetura do sistema.                                              | Must       |
| **RNF39** | Integridade da telemetria               | Os dados apresentados na interface web devem corresponder aos dados transmitidos pelo Micromouse, sem alterações indevidas durante a transmissão ou processamento.                                    | Must       |
| **RNF40** | Adaptação da representação do labirinto | A representação visual do labirinto deve se adaptar aos três tamanhos previstos, mantendo todas as células e o trajeto visíveis.                                                                      | Should     |
| **RNF41** | Legibilidade da telemetria              | A interface web deve apresentar separadamente, no mínimo, os seis dados obrigatórios: tipo de labirinto, trajeto, consumo da bateria, velocidade média, tempo de execução e resultado do desafio. | Must       |
| **RNF42** | Responsividade da interface             | A interface web deve se adaptar aos tamanhos de tela utilizados pelo projeto sem sobreposição ou ocultação dos dados obrigatórios.                                                                    | Should     |
| **RNF43** | Restrição de recursos                   | As tecnologias utilizadas no sistema devem ser compatíveis com os recursos técnicos, materiais e financeiros disponíveis para o projeto.                                                              | Must       |

---

## ÉPICO 6 — Banco de Dados e Histórico

| ID        | Requisito                         | Descrição                                                                                                                        | Prioridade |
| --------- | --------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| **RNF44** | Persistência dos dados            | Os dados de uma execução concluída devem permanecer armazenados e disponíveis para consultas posteriores.                        | Must       |
| **RNF45** | Integridade dos dados armazenados | Os dados armazenados devem preservar corretamente a associação entre execução, labirinto, telemetria e resultado correspondente. | Must       |

---

## ÉPICO 7 — Integração e Validação


| ID  | Requisito | Descrição | Prioridade |   
| --------- | --------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ---------- |


## Histórico de Versões


| Versão |Descrição     |Autor                                       |Data    |Revisor|
|:-:     | :-:          | :-:                                        | :-:        |:-:|
|1.0     |Criação da documento| Equipe de Software | 27/09/2026 | [Letícia Monteiro](https://github.com/LeticiaMonteiroo) |