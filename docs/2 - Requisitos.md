# Requisitos

> Documento que descreve as funcionalidades, restrições e critérios técnicos que orientam o desenvolvimento e a validação do micromouse Rato Cego.

## Visão Geral dos Requisitos

Este documento apresenta os requisitos funcionais (RF) e não funcionais (RNF) do projeto **Rato Cego**, organizados de acordo com os épicos definidos para o desenvolvimento do Micromouse.

Os requisitos servem como base para o planejamento, desenvolvimento, integração e validação do sistema.

---

# Requisitos Funcionais

## ÉPICO 1 — Estrutura do Micromouse

| ID                                      | Requisito                                                                                                                                                                                   | Prioridade |
| --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rf1"></a> **[RF1](HUs.md#hu01)** | **Proteção dos componentes internos** — A estrutura deve proteger os componentes eletrônicos, sensores, motores e demais módulos contra choques, colisões e impactos durante a operação.    | Must       |
| <a id="rf2"></a> **[RF2](HUs.md#hu02)** | **Acesso aos componentes internos** — A estrutura deve permitir acesso aos componentes para inspeção, manutenção, substituição e ajustes sem exigir a desmontagem completa do chassi.       | Should     |
| <a id="rf3"></a> **[RF3](HUs.md#hu03)** | **Fixação dos subsistemas** — A estrutura deve possuir pontos de fixação adequados para os subsistemas de hardware, alimentação e controle, evitando deslocamentos durante a operação.      | Must       |
| <a id="rf4"></a> **[RF4](HUs.md#hu04)** | **Modularidade estrutural** — A estrutura deve permitir a substituição ou atualização de módulos sem exigir a reconstrução completa do chassi.                                              | Should     |
| <a id="rf5"></a> **[RF5](HUs.md#hu05)** | **Fixação e alinhamento do sistema de locomoção** — Os motores, rodas e demais elementos de tração devem ser fixados de forma a manter seu posicionamento e alinhamento durante a operação. | Must       |
| <a id="rf6"></a> **[RF6](HUs.md#hu06)** | **Acomodação e posicionamento dos sensores** — A estrutura deve possuir espaços e pontos de fixação adequados para os sensores, mantendo sua orientação e evitando obstruções.              | Must       |
| <a id="rf7"></a> **[RF7](HUs.md#hu07)** | **Organização e proteção do cabeamento** — Os cabos devem ser organizados e protegidos de forma a evitar interferência com rodas, motores, sensores e demais componentes móveis.            | Must       |
| <a id="rf8"></a> **[RF8](HUs.md#hu08)** | **Acesso aos elementos de operação** — Os elementos externos de operação, como interruptores, conectores e interfaces, devem permanecer acessíveis durante a utilização do Micromouse.      | Should     |

---

## ÉPICO 2 — Hardware e Sensoriamento

| ID                                        | Requisito                                                                                                                                                                                                                                                             | Prioridade |
| ----------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rf9"></a> **[RF9](HUs.md#hu09)**   | **Reconhecimento Espacial e Detecção de Obstáculos** — O Micromouse deve realizar amostragem contínua do ambiente por meio dos sensores de proximidade, identificando paredes de aproximadamente 5 cm de altura e aberturas nas direções frontal, lateral e diagonal. | Must       |
| <a id="rf10"></a> **[RF10](HUs.md#hu10)** | **Processamento Embarcado Autônomo** — O microcontrolador deve executar localmente a leitura dos sensores, a lógica de controle e o armazenamento das informações necessárias à navegação, sem depender de processamento externo.                                     | Must       |
| <a id="rf11"></a> **[RF11](HUs.md#hu11)** | **Acionamento e Modulação de Potência Motriz** — O sistema deve converter os comandos lógicos em acionamento elétrico reversível dos motores de tração, permitindo controle contínuo da velocidade por PWM.                                                           | Must       |
| <a id="rf12"></a> **[RF12](HUs.md#hu12)** | **Manutenção de Trajetória Centralizada** — O sistema deve ajustar diferencialmente o acionamento dos motores para manter o Micromouse em trajetória reta e centralizada entre as paredes do labirinto.                                                               | Must       |
| <a id="rf13"></a> **[RF13](HUs.md#hu13)** | **Execução de Curvas e Manobras de Rotação** — O sistema deve permitir a execução de curvas de 90° e 180° por meio do acionamento diferencial dos motores, respeitando as dimensões das células de 18 × 18 cm.                                                        | Must       |
| <a id="rf14"></a> **[RF14](HUs.md#hu14)** | **Transmissão Sem Fio de Telemetria** — O Micromouse deve possuir um meio de comunicação sem fio capaz de transmitir os dados necessários à telemetria durante a execução.                                                                                            | Must       |
| <a id="rf15"></a> **[RF15](HUs.md#hu15)** | **Regulação e Distribuição de Energia** — O sistema deve receber energia da fonte recarregável e distribuir tensões reguladas e estáveis aos circuitos de controle, sensores e atuadores.                                                                             | Must       |
| <a id="rf16"></a> **[RF16](HUs.md#hu16)** | **Interface Física de Operação e Disparo** — O Micromouse deve possuir uma interface física externa que permita iniciar e interromper manualmente a execução.                                                                                                         | Must       |

---

## ÉPICO 3 — Alimentação e Energia

| ID                                        | Requisito                                                                                                                                                                                                       | Prioridade |
| ----------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rf17"></a> **[RF17](HUs.md#hu17)** | **Telemetria energética em tempo real** — Durante cada execução, o sistema deve coletar e disponibilizar dados sobre o consumo e a condição da bateria, associando essas informações à execução correspondente. | Must       |
| <a id="rf18"></a> **[RF18](HUs.md#hu18)** | **Isolamento manual da alimentação** — O circuito de alimentação deve possuir um interruptor físico acessível capaz de desconectar a bateria dos subsistemas durante transporte, montagem ou manutenção.        | Must       |
| <a id="rf19"></a> **[RF19](HUs.md#hu19)** | **Aviso web de bateria baixa** — O firmware deve monitorar periodicamente a tensão da bateria e, ao atingir o nível crítico definido, enviar um aviso à interface web durante a execução.                       | Must       |
| <a id="rf20"></a> **[RF20](HUs.md#hu20)** | **Bateria removível e reinstalável** — A bateria deve poder ser removida e reinstalada sem exigir a desmontagem completa do chassi.                                                                             | Could      |

---

## ÉPICO 4 — Navegação e Controle

| ID                                        | Requisito                                                                                                                                                                                                                | Prioridade |
| ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| <a id="rf21"></a> **[RF21](HUs.md#hu21)** | **Identificação de paredes** — O software deve identificar a presença de paredes a partir dos dados obtidos pelos sensores.                                                                                              | Must       |
| <a id="rf22"></a> **[RF22](HUs.md#hu22)** | **Localização no labirinto** — O sistema deve acompanhar a localização do Micromouse dentro do labirinto durante a execução.                                                                                             | Must       |
| <a id="rf23"></a> **[RF23](HUs.md#hu23)** | **Determinação do percurso** — O sistema deve determinar autonomamente os movimentos necessários com base nas informações obtidas durante a execução.                                                                    | Must       |
| <a id="rf24"></a> **[RF24](HUs.md#hu24)** | **Navegação autônoma** — O Micromouse deve realizar a navegação no labirinto sem intervenção humana durante a execução.                                                                                                  | Must       |
| <a id="rf25"></a> **[RF25](HUs.md#hu25)** | **Identificação do objetivo** — O sistema deve identificar quando o Micromouse alcançar a região definida como objetivo do labirinto.                                                                                    | Must       |
| <a id="rf26"></a> **[RF26](HUs.md#hu26)** | **Registro do trajeto** — O sistema deve registrar o trajeto percorrido pelo Micromouse durante cada execução.                                                                                                           | Must       |
| <a id="rf27"></a> **[RF27](HUs.md#hu27)** | **Seleção do tipo de labirinto** — O sistema deve permitir ao operador selecionar o tipo de labirinto antes do início da execução.                                                                                       | Must       |
| <a id="rf28"></a> **[RF28](HUs.md#hu28)** | **Preparação da execução** — O sistema deve verificar as condições necessárias para iniciar uma execução, incluindo a seleção do labirinto e a disponibilidade dos recursos necessários.                                 | Must       |
| <a id="rf29"></a> **[RF29](HUs.md#hu29)** | **Início do percurso** — O sistema deve permitir ao operador iniciar o percurso após a preparação da execução.                                                                                                           | Must       |
| <a id="rf30"></a> **[RF30](HUs.md#hu30)** | **Interrupção do percurso** — O sistema deve permitir ao operador interromper uma execução em andamento por meio da interface disponível.                                                                                | Must       |
| <a id="rf31"></a> **[RF31](HUs.md#hu31)** | **Reinício de execução** — O sistema deve permitir iniciar uma nova execução após a conclusão ou interrupção de uma execução anterior.                                                                                   | Should     |
| <a id="rf32"></a> **[RF32](HUs.md#hu32)** | **Gerenciamento do estado da execução** — O sistema deve controlar e disponibilizar os estados da execução, incluindo, no mínimo, **aguardando, em execução, concluída, interrompida e encerrada por falha ou timeout**. | Must       |
| <a id="rf33"></a> **[RF33](HUs.md#hu33)** | **Encerramento automático da execução** — O sistema deve encerrar automaticamente a execução quando o objetivo for alcançado, o tempo máximo for excedido ou ocorrer uma falha que impeça a continuidade segura.         | Must       |
| <a id="rf34"></a> **[RF34](HUs.md#hu34)** | **Parada segura** — Quando uma execução for interrompida ou ocorrer uma condição de falha que exija parada, o sistema deve interromper ou limitar o acionamento dos motores de forma segura.                             | Must       |
| <a id="rf35"></a> **[RF35](HUs.md#hu35)** | **Medição de deslocamento** — O sistema deve utilizar os encoders dos motores para obter informações de deslocamento e velocidade do Micromouse.                                                                         | Must       |

---

## ÉPICO 5 — Sistema Web e Telemetria

| ID                                        | Requisito                                                                                                                                                                                             | Prioridade |
| ----------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rf36"></a> **[RF36](HUs.md#hu36)** | **Recepção e disponibilização da telemetria** — O sistema web deve receber os dados de telemetria enviados pelo Micromouse e disponibilizá-los para acompanhamento da execução.                       | Must       |
| <a id="rf37"></a> **[RF37](HUs.md#hu37)** | **Visualização do trajeto** — A interface web deve apresentar e atualizar o trajeto percorrido pelo Micromouse durante a execução.                                                                    | Must       |
| <a id="rf38"></a> **[RF38](HUs.md#hu38)** | **Monitoramento da bateria** — A interface web deve apresentar a condição e o consumo da bateria durante a execução, utilizando os dados fornecidos pela telemetria energética.                       | Must       |
| <a id="rf39"></a> **[RF39](HUs.md#hu39)** | **Monitoramento do tempo** — O sistema deve contabilizar o tempo de execução e apresentá-lo na interface web durante o percurso.                                                                      | Must       |
| <a id="rf40"></a> **[RF40](HUs.md#hu40)** | **Exibição da velocidade média** — A interface web deve calcular e apresentar a velocidade média da execução.                                                                                         | Must       |
| <a id="rf41"></a> **[RF41](HUs.md#hu41)** | **Resultado do desafio** — A interface web deve informar se o Micromouse concluiu ou não o desafio ao final da execução.                                                                              | Must       |
| <a id="rf42"></a> **[RF42](HUs.md#hu42)** | **Estabelecimento da conexão WebSocket** — O sistema deve estabelecer uma conexão WebSocket entre a aplicação web e o serviço responsável pela comunicação em tempo real.                             | Must       |
| <a id="rf43"></a> **[RF43](HUs.md#hu43)** | **Detecção de perda de conexão WebSocket** — O sistema deve detectar a perda ou indisponibilidade da conexão WebSocket durante uma execução.                                                          | Must       |
| <a id="rf44"></a> **[RF44](HUs.md#hu44)** | **Reconexão WebSocket** — O sistema deve realizar tentativas automáticas de reconexão WebSocket após uma perda de conexão, respeitando a política de tentativas definida pelo projeto.                | Should     |
| <a id="rf45"></a> **[RF45](HUs.md#hu45)** | **Sinalização de indisponibilidade** — A interface web deve informar ao operador quando uma comunicação necessária para o monitoramento estiver indisponível.                                         | Must       |
| <a id="rf46"></a> **[RF46](HUs.md#hu46)** | **Comunicação MQTT** — O sistema deve utilizar MQTT para transmissão e recepção das mensagens definidas pela arquitetura de comunicação do projeto.                                                   | Must       |
| <a id="rf47"></a> **[RF47](HUs.md#hu47)** | **Reconexão MQTT** — O sistema deve realizar tentativas de reconexão MQTT após uma perda de conexão.                                                                                                  | Must       |
| <a id="rf48"></a> **[RF48](HUs.md#hu48)** | **Tratamento de mensagens MQTT** — O sistema deve receber, validar e processar as mensagens MQTT de acordo com o formato definido para a comunicação, descartando ou sinalizando mensagens inválidas. | Must       |

---

## ÉPICO 6 — Banco de Dados e Histórico

| ID                                        | Requisito                                                                                                                                                                                                         | Prioridade |
| ----------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rf49"></a> **[RF49](HUs.md#hu49)** | **Armazenamento da execução** — Após cada execução, o sistema deve armazenar os dados coletados durante o percurso no banco de dados.                                                                             | Must       |
| <a id="rf50"></a> **[RF50](HUs.md#hu50)** | **Associação ao labirinto** — Os dados armazenados devem ser associados ao tipo de labirinto correspondente à execução.                                                                                           | Must       |
| <a id="rf51"></a> **[RF51](HUs.md#hu51)** | **Consulta por labirinto** — A interface web deve permitir consultar os dados das execuções realizadas em um determinado labirinto.                                                                               | Must       |
| <a id="rf52"></a> **[RF52](HUs.md#hu52)** | **Consulta geral** — A interface web deve permitir consultar conjuntamente os dados armazenados de diferentes labirintos e execuções.                                                                             | Must       |
| <a id="rf53"></a> **[RF53](HUs.md#hu53)** | **Registro de falhas da execução** — O sistema deve registrar eventos relevantes que impeçam ou interrompam uma execução, incluindo falhas de comunicação, timeout, interrupções e condições de falha detectadas. | Must       |

---

## ÉPICO 7 — Integração e Validação

### Requisitos Funcionais

| ID                                        | Requisito                                                                                                                                                                                  | Prioridade |
| ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| <a id="rf54"></a> **[RF54](HUs.md#hu54)** | **Correção da movimentação** — O sistema deve utilizar as informações de deslocamento obtidas pelos encoders para auxiliar no controle e posicionamento do Micromouse durante a navegação. | Must       |
| <a id="rf55"></a> **[RF55](HUs.md#hu55)** | **Calibração dos sensores** — O sistema deve permitir a calibração dos sensores antes da operação, utilizando procedimentos definidos pelo projeto.                                        | Should     |
| <a id="rf56"></a> **[RF56](HUs.md#hu56)** | **Sinalização do ciclo de recarga** — Durante o processo de recarga, o Micromouse poderá indicar visualmente o estado do ciclo de carregamento.                                            | Could      |

---

# Requisitos Não Funcionais

## ÉPICO 1 — Estrutura do Micromouse

| ID                           | Requisito                                                                                                                                                                                                              | Prioridade |
| ---------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rnf1"></a> **RNF1**   | **Limite dimensional do chassi** — O Micromouse deve possuir dimensões máximas de **16,5 cm × 16,5 cm**, considerando qualquer estado operacional permitido pelo projeto.                                              | Must       |
| <a id="rnf2"></a> **RNF2**   | **Compatibilidade com as dimensões do labirinto** — A geometria do Micromouse deve permitir movimentação livre nas células de **18 cm**, sem pontos salientes ou interferências que possam causar travamento ou danos. | Must       |
| <a id="rnf3"></a> **RNF3**   | **Massa estrutural** — A massa da estrutura deve permanecer dentro do limite definido pelo projeto, sem comprometer aceleração, frenagem, estabilidade ou autonomia.                                                   | Must       |
| <a id="rnf4"></a> **RNF4**   | **Resistência mecânica** — A estrutura deve suportar movimentação, aceleração, frenagem e impactos previstos durante os testes sem sofrer deformações que comprometam seu funcionamento.                               | Must       |
| <a id="rnf5"></a> **RNF5**   | **Rigidez estrutural** — A estrutura deve manter sua geometria e o posicionamento dos componentes durante a operação, evitando deformações ou folgas que prejudiquem a navegação.                                      | Must       |
| <a id="rnf6"></a> **RNF6**   | **Estabilidade estrutural** — O Micromouse deve permanecer estável durante acelerações, frenagens, curvas e mudanças de direção.                                                                                       | Must       |
| <a id="rnf7"></a> **RNF7**   | **Distribuição de massa** — A distribuição de massa deve evitar desequilíbrios que prejudiquem a locomoção, estabilidade ou precisão das manobras.                                                                     | Should     |
| <a id="rnf8"></a> **RNF8**   | **Compatibilidade entre subsistemas** — As dimensões, espaços e pontos de fixação da estrutura devem ser compatíveis com os componentes de hardware, alimentação, sensores e demais subsistemas.                       | Must       |
| <a id="rnf9"></a> **RNF9**   | **Precisão dimensional de fabricação** — As dimensões finais da estrutura devem permanecer dentro das tolerâncias de fabricação definidas no projeto.                                                                  | Must       |
| <a id="rnf10"></a> **RNF10** | **Segurança estrutural** — A estrutura não deve apresentar pontas, arestas ou elementos expostos que possam causar danos aos componentes ou comprometer a operação.                                                    | Must       |
| <a id="rnf11"></a> **RNF11** | **Durabilidade da estrutura** — A estrutura deve manter suas características mecânicas e dimensões dentro das tolerâncias estabelecidas após os testes e execuções previstos.                                          | Should     |
| <a id="rnf12"></a> **RNF12** | **Facilidade de montagem e desmontagem** — A estrutura deve permitir montagem, desmontagem e manutenção utilizando as ferramentas disponíveis, sem procedimentos excessivamente complexos.                             | Should     |
| <a id="rnf13"></a> **RNF13** | **Aproveitamento do espaço interno** — O espaço interno deve ser utilizado de forma eficiente, sem comprometer circulação, manutenção, ventilação ou integração dos subsistemas.                                       | Should     |
| <a id="rnf14"></a> **RNF14** | **Compatibilidade com os materiais disponíveis** — Os materiais utilizados devem ser compatíveis com os processos de fabricação, ferramentas, orçamento e recursos disponíveis no projeto.                             | Must       |

---

## ÉPICO 2 — Hardware e Sensoriamento

| ID                           | Requisito                                                                                                                                                                                                                        | Prioridade |
| ---------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rnf15"></a> **RNF15** | **Proteção e margem de segurança elétrica** — O sistema deve possuir proteção contra inversão de polaridade, e os condutores devem suportar pelo menos **30% acima da corrente máxima prevista** para o circuito correspondente. | Must       |
| <a id="rnf16"></a> **RNF16** | **Estabilidade de tensão lógica** — Os reguladores devem manter as linhas de **3,3 V e 5 V dentro de ±5%** de seus valores nominais durante condições de operação, incluindo a partida dos motores.                              | Must       |
| <a id="rnf17"></a> **RNF17** | **Latência da malha física de resposta** — O intervalo entre a leitura de um sensor e o acionamento efetivo correspondente dos motores deve ser de, no máximo, **50 ms**.                                                        | Must       |

---

## ÉPICO 3 — Alimentação e Energia

| ID                           | Requisito                                                                                                                                                                                                          | Prioridade |
| ---------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| <a id="rnf18"></a> **RNF18** | **Autonomia operacional** — Com sensores, controle, motores e telemetria ativos, o Micromouse deve operar por pelo menos **30 minutos sem recarga ou substituição da bateria**.                                    | Must       |
| <a id="rnf19"></a> **RNF19** | **Rendimento da conversão de energia** — Os conversores e reguladores devem apresentar eficiência mínima de **85% em carga nominal**.                                                                              | Should     |
| <a id="rnf20"></a> **RNF20** | **Consumo em inatividade** — Com o sistema energizado, mas sem executar um percurso, o consumo total deve ser de no máximo **50 mA**, considerando a desativação de periféricos desnecessários.                    | Should     |
| <a id="rnf21"></a> **RNF21** | **Aviso preventivo de descarga** — O sistema deve emitir o aviso de bateria baixa quando qualquer célula atingir **3,3 V**, antes de atingir uma faixa considerada prejudicial à bateria.                          | Must       |
| <a id="rnf22"></a> **RNF22** | **Restrição de massa do conjunto energético** — Os componentes destinados à alimentação devem representar no máximo **25% da massa final do Micromouse**.                                                          | Must       |
| <a id="rnf23"></a> **RNF23** | **Integridade da alimentação dos sensores** — Durante acelerações e partidas dos motores, a alimentação dos sensores deve permanecer entre **95% e 105% da tensão nominal**.                                       | Must       |
| <a id="rnf24"></a> **RNF24** | **Proteção contra falhas elétricas** — O sistema de alimentação deve interromper ou limitar a corrente em condições de sobrecorrente ou curto-circuito.                                                            | Must       |
| <a id="rnf25"></a> **RNF25** | **Confiabilidade da telemetria energética** — Após a calibração, as leituras de tensão e consumo devem apresentar erro máximo de **5% em relação ao instrumento de referência**.                                   | Should     |
| <a id="rnf26"></a> **RNF26** | **Compatibilidade de tensão dos subsistemas** — A tensão nominal e a faixa de descarga da bateria devem ser compatíveis com todos os subsistemas, e os reguladores devem suportar as tensões e correntes exigidas. | Must       |

---

## ÉPICO 4 — Navegação e Controle

| ID                           | Requisito                                                                                                                                                                                                                                                                         | Prioridade |
| ---------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rnf27"></a> **RNF27** | **Tempo máximo de execução** — Cada tentativa de resolução do labirinto deve ser encerrada em no máximo **10 minutos**.                                                                                                                                                           | Must       |
| <a id="rnf28"></a> **RNF28** | **Compatibilidade com os labirintos** — O software de navegação deve ser compatível com os labirintos de **4 × 4, 8 × 4 e 12 × 4 células**, considerando células de **18 cm**.                                                                                                    | Must       |
| <a id="rnf29"></a> **RNF29** | **Compatibilidade com o hardware** — O software embarcado deve ser compatível com os componentes eletrônicos utilizados para sensoriamento, navegação e comunicação.                                                                                                              | Must       |
| <a id="rnf30"></a> **RNF30** | **Compatibilidade com o microcontrolador** — O software embarcado deve ser executável no **ESP32** definido para o projeto.                                                                                                                                                       | Must       |
| <a id="rnf31"></a> **RNF31** | **Timeout da conexão WebSocket** — O sistema deve detectar a ausência de comunicação válida no WebSocket após, no máximo, **X segundos** sem recebimento de mensagem ou heartbeat.                                                                                                | Must       |
| <a id="rnf32"></a> **RNF32** | **Tempo de reconexão WebSocket** — Após a perda de conexão, o sistema deve realizar novas tentativas de conexão a cada **X segundos**, durante no máximo **Y tentativas**, conforme os valores definidos pela equipe.                                                             | Should     |
| <a id="rnf33"></a> **RNF33** | **Timeout e Keep-Alive MQTT** — A comunicação MQTT deve utilizar mecanismo de keep-alive com intervalo máximo de **X segundos**, considerando a conexão perdida após **Y segundos** sem comunicação válida.                                                                       | Must       |
| <a id="rnf34"></a> **RNF34** | **Reconexão MQTT** — Após a perda da conexão MQTT, o sistema deve realizar tentativas automáticas de reconexão em intervalos de **X segundos**, durante no máximo **Y tentativas**.                                                                                               | Must       |
| <a id="rnf35"></a> **RNF35** | **Integridade das mensagens** — As mensagens trocadas pelo sistema devem seguir o formato definido pela arquitetura de comunicação. Mensagens inválidas, incompletas ou incompatíveis devem ser identificadas e descartadas ou sinalizadas sem provocar comportamento inesperado. | Must       |
| <a id="rnf36"></a> **RNF36** | **Latência da telemetria** — Os dados de telemetria devem estar disponíveis para a interface web em até **X ms** após sua geração no sistema embarcado, durante uma execução normal.                                                                                              | Must       |
| <a id="rnf37"></a> **RNF37** | **Precisão de sensoriamento** — Os sensores utilizados para navegação devem apresentar precisão suficiente para identificar paredes, obstáculos e aberturas dentro das tolerâncias estabelecidas nos testes do projeto.                                                           | Should     |

---

## ÉPICO 5 — Sistema Web e Telemetria

| ID                           | Requisito                                                                                                                                                                                                                          | Prioridade |
| ---------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| <a id="rnf38"></a> **RNF38** | **Atualização da telemetria** — A interface web deve atualizar os dados de telemetria continuamente durante uma execução, respeitando a frequência definida pela arquitetura do sistema.                                           | Must       |
| <a id="rnf39"></a> **RNF39** | **Integridade da telemetria** — Os dados apresentados na interface web devem corresponder aos dados transmitidos pelo Micromouse, sem alterações indevidas durante a transmissão ou processamento.                                 | Must       |
| <a id="rnf40"></a> **RNF40** | **Adaptação da representação do labirinto** — A representação visual do labirinto deve se adaptar aos três tamanhos previstos, mantendo todas as células e o trajeto visíveis.                                                     | Should     |
| <a id="rnf41"></a> **RNF41** | **Legibilidade da telemetria** — A interface web deve apresentar separadamente, no mínimo, os seis dados obrigatórios: tipo de labirinto, trajeto, consumo da bateria, velocidade média, tempo de execução e resultado do desafio. | Must       |
| <a id="rnf42"></a> **RNF42** | **Responsividade da interface** — A interface web deve se adaptar aos tamanhos de tela utilizados pelo projeto sem sobreposição ou ocultação dos dados obrigatórios.                                                               | Should     |
| <a id="rnf43"></a> **RNF43** | **Restrição de recursos** — As tecnologias utilizadas no sistema devem ser compatíveis com os recursos técnicos, materiais e financeiros disponíveis para o projeto.                                                               | Must       |

---

## ÉPICO 6 — Banco de Dados e Histórico

| ID                           | Requisito                                                                                                                                                                | Prioridade |
| ---------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| <a id="rnf44"></a> **RNF44** | **Persistência dos dados** — Os dados de uma execução concluída devem permanecer armazenados e disponíveis para consultas posteriores.                                   | Must       |
| <a id="rnf45"></a> **RNF45** | **Integridade dos dados armazenados** — Os dados armazenados devem preservar corretamente a associação entre execução, labirinto, telemetria e resultado correspondente. | Must       |

---

## ÉPICO 7 — Integração e Validação

A validação dos requisitos de integração será realizada por meio dos testes definidos nas respectivas Issues do projeto, considerando os requisitos funcionais e não funcionais relacionados aos subsistemas envolvidos.

---

# Histórico de Versões

| Versão | Descrição            | Autor              |    Data    | Revisor                                                 |
| :----: | -------------------- | ------------------ | :--------: | ------------------------------------------------------- |
|   1.0  | Criação do documento | Equipe de Software | 27/09/2026 | [Letícia Monteiro](https://github.com/LeticiaMonteiroo) |

