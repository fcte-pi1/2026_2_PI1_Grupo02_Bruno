# Requisitos

Este documento consolida os requisitos do projeto Rato Cego, um robô móvel autônomo capaz de mapear e solucionar labirintos. Os requisitos orientam as decisões das equipes de Hardware, Estruturas, Energia e Software, assegurando que os subsistemas sejam desenvolvidos de forma integrada e atendam aos objetivos definidos no Termo de Abertura do Projeto.

Cada requisito possui um identificador único, uma descrição objetiva e uma prioridade. O acompanhamento da implementação, dos responsáveis, das histórias de usuário, dos critérios de aceitação e das tarefas relacionadas será realizado no GitHub Projects.

## Épicos do Projeto

Os requisitos do projeto estão organizados nos seguintes épicos:

1. **Épico 1 — Estrutura do Micromouse**
2. **Épico 2 — Hardware e Sensoriamento**
3. **Épico 3 — Alimentação e Energia**
4. **Épico 4 — Navegação e Controle**
5. **Épico 5 — Sistema Web e Telemetria**
6. **Épico 6 — Banco de Dados e Histórico**
7. **Épico 7 — Integração e Validação**

Os épicos representam agrupamentos funcionais do produto e não correspondem necessariamente às equipes responsáveis por sua implementação. Um mesmo épico pode envolver diferentes áreas técnicas, como Software, Hardware, Energia e Estruturas.

---

# Requisitos Funcionais (RF)

Os **Requisitos Funcionais (RF)** definem os comportamentos, funções e serviços que o micromouse e os sistemas associados devem fornecer.

Eles descrevem **o que o sistema deve fazer**, incluindo ações realizadas pelo robô, pelo software embarcado, pelo sistema web e pelos demais componentes da solução.

A prioridade de cada requisito é definida pela classificação MoSCoW:

* **Must:** requisito indispensável para a entrega e validação do projeto.
* **Should:** requisito importante, a ser implementado quando houver viabilidade técnica e de prazo.
* **Could:** requisito desejável, implementado caso não comprometa os itens de maior prioridade.

## ÉPICO 1 — Estrutura do Micromouse

### Área: Estruturas

| ID  | Requisito                                     | Descrição                                                                                                                                                    | Prioridade |
| --- | --------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| RF1 | Proteção dos componentes internos             | O produto deve proteger os componentes eletrônicos, sensores, motores e demais módulos contra choques e colisões durante a operação.                         | Must       |
| RF2 | Acesso aos componentes internos               | O produto deve permitir acesso aos componentes internos para inspeção, manutenção, substituição e ajustes sem necessidade de desmontagem completa do chassi. | Should     |
| RF3 | Fixação dos subsistemas                       | O produto deve possuir pontos de fixação adequados para os componentes de hardware, energia e software embarcado, evitando deslocamentos durante a operação. | Must       |
| RF4 | Modularidade estrutural                       | O produto deve permitir a substituição ou atualização de módulos de forma independente, sem necessidade de reconstrução completa do chassi.                  | Should     |
| RF5 | Fixação e alinhamento do sistema de locomoção | O produto deve permitir a fixação dos motores, rodas e demais elementos de tração, mantendo sua posição e alinhamento durante a operação.                    | Must       |
| RF6 | Acomodação e posicionamento dos sensores      | O produto deve possuir espaços e pontos de fixação para os sensores, mantendo sua orientação adequada e evitando obstruções.                                 | Must       |
| RF7 | Organização e proteção do cabeamento          | O produto deve organizar e proteger os cabos de modo que não interfiram nas rodas, motores, sensores ou demais componentes.                                  | Must       |
| RF8 | Acesso aos elementos de operação              | O produto deve permitir acesso externo ao interruptor de alimentação, conectores e demais interfaces necessárias para operação e manutenção.                 | Should     |

## ÉPICO 2 — Hardware e Sensoriamento

### Área: Hardware

| ID   | Requisito                                        | Descrição                                                                                                                                                                              | Prioridade |
| ---- | ------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RF9  | Reconhecimento Espacial e Detecção de Obstáculos | O sistema deve realizar amostragem contínua do ambiente por meio de sensores de proximidade, permitindo detectar paredes de 5 cm de altura e aberturas frontais, laterais e diagonais. | Must       |
| RF10 | Processamento Embarcado Autônomo                 | O microcontrolador deve executar localmente a leitura dos sensores, a lógica de controle e o armazenamento do mapa, sem depender de processamento externo.                             | Must       |
| RF11 | Acionamento e Modulação de Potência Motriz       | O sistema deve converter comandos lógicos em acionamento elétrico reversível, permitindo modulação contínua da velocidade por PWM para os motores de tração.                           | Must       |
| RF12 | Manutenção de Trajetória Centralizada            | O sistema deve ajustar diferencialmente o acionamento dos motores para manter o micromouse em trajetória reta e centralizada entre as paredes do labirinto.                            | Must       |
| RF13 | Execução de Curvas e Manobras de Rotação         | O sistema deve permitir a execução de curvas e rotações precisas de 90° e 180° sobre o próprio eixo, compatíveis com células de 18 × 18 cm.                                            | Must       |
| RF14 | Transmissão Sem Fio de Telemetria                | O sistema deve possuir um canal sem fio para transmitir os dados da execução em tempo real ao sistema web.                                                                             | Must       |
| RF15 | Regulação e Distribuição de Energia              | O sistema deve receber a alimentação da fonte recarregável e distribuir tensões reguladas e estáveis aos componentes eletrônicos, sensores e atuadores.                                | Must       |
| RF16 | Interface Física de Operação e Disparo           | O produto deve possuir um interruptor ou botão externo acessível para permitir o início manual e a interrupção da navegação.                                                           | Must       |

## ÉPICO 3 — Alimentação e Energia

### Área: Energia

| ID   | Requisito                           | Descrição                                                                                                                                                           | Prioridade |
| ---- | ----------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RF17 | Telemetria energética em tempo real | Durante cada execução, o sistema web deve apresentar o consumo da bateria em tempo real e associá-lo à execução correspondente.                                     | Must       |
| RF18 | Isolamento manual da alimentação    | O circuito de alimentação deve possuir um interruptor físico acessível que permita desconectar a bateria dos subsistemas durante transporte, montagem e manutenção. | Must       |
| RF19 | Aviso web de bateria baixa          | O firmware deve monitorar periodicamente a tensão da bateria e, ao atingir um nível crítico, emitir um aviso ao sistema web durante a execução.                     | Must       |
| RF20 | Bateria removível e reinstalável    | A bateria deve poder ser removida e reinstalada sem a necessidade de desmontagem completa do chassi.                                                                | Could      |

## ÉPICO 4 — Navegação e Controle

### Área: Software

| ID   | Requisito                           | Descrição                                                                                                                                                      | Prioridade |
| ---- | ----------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RF21 | Identificação de paredes            | O software deve identificar paredes a partir dos dados recebidos dos sensores.                                                                                 | Must       |
| RF22 | Localização no labirinto            | O sistema deve monitorar a localização do robô no labirinto durante a execução.                                                                                | Must       |
| RF23 | Determinação do percurso            | O sistema deve determinar autonomamente os movimentos necessários com base nas informações obtidas durante a execução.                                         | Must       |
| RF24 | Navegação autônoma                  | O robô deve navegar pelo labirinto sem intervenção humana durante a execução.                                                                                  | Must       |
| RF25 | Identificação do objetivo           | O sistema deve identificar quando o robô atingir a área objetivo do labirinto.                                                                                 | Must       |
| RF26 | Registro do trajeto                 | O sistema deve registrar o trajeto percorrido pelo robô durante cada execução.                                                                                 | Must       |
| RF27 | Seleção do tipo de labirinto        | O sistema web deve permitir que o operador selecione o tipo de labirinto antes do início da execução.                                                          | Must       |
| RF28 | Configuração da execução            | O sistema deve permitir a configuração dos parâmetros necessários para uma execução, incluindo o tipo de labirinto.                                            | Must       |
| RF29 | Início do percurso                  | O operador deve poder iniciar a execução após a configuração dos parâmetros necessários.                                                                       | Must       |
| RF30 | Interrupção do percurso             | O operador deve poder interromper uma execução em andamento, fazendo com que o micromouse interrompa sua navegação de forma segura.                            | Must       |
| RF31 | Reinício de execução                | O sistema deve permitir o início de uma nova execução após o término ou interrupção de uma execução anterior.                                                  | Should     |
| RF32 | Gerenciamento do estado da execução | O sistema deve controlar os estados da execução, incluindo, no mínimo, aguardando, em execução, concluída, interrompida e encerrada por falha ou timeout.      | Must       |
| RF33 | Encerramento automático da execução | O sistema deve encerrar automaticamente a execução quando o objetivo for atingido, o tempo máximo for excedido ou ocorrer uma falha que impeça a continuidade. | Must       |
| RF34 | Parada segura                       | Em situações de interrupção, perda de comunicação ou falha detectada, o sistema deve interromper ou limitar o acionamento dos motores de forma segura.         | Must       |
| RF35 | Medição de deslocamento             | O sistema deve utilizar os encoders dos motores para obter informações relacionadas ao deslocamento e à velocidade do robô.                                    | Must       |

## ÉPICO 5 — Sistema Web e Telemetria

### Área: Software

| ID   | Requisito                              | Descrição                                                                                                                                       | Prioridade |
| ---- | -------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RF36 | Transmissão de telemetria              | O robô deve transmitir os dados de desempenho necessários para o monitoramento pelo sistema web.                                                | Must       |
| RF37 | Visualização do trajeto                | O sistema web deve apresentar e atualizar o trajeto percorrido pelo robô em tempo real.                                                         | Must       |
| RF38 | Monitoramento da bateria               | O sistema web deve apresentar e atualizar o consumo da bateria em tempo real.                                                                   | Must       |
| RF39 | Monitoramento do tempo                 | O sistema deve contabilizar o tempo de execução e apresentá-lo no sistema web durante a execução.                                               | Must       |
| RF40 | Exibição da velocidade média           | O sistema web deve calcular e apresentar a velocidade média da execução.                                                                        | Must       |
| RF41 | Resultado do desafio                   | O sistema web deve informar se o robô concluiu o desafio.                                                                                       | Must       |
| RF42 | Estabelecimento da conexão WebSocket   | O sistema deve estabelecer uma conexão WebSocket entre o sistema web e o serviço responsável pela comunicação em tempo real durante a execução. | Must       |
| RF43 | Detecção de perda de conexão WebSocket | O sistema deve detectar a perda ou indisponibilidade da conexão WebSocket durante a execução.                                                   | Must       |
| RF44 | Reconexão WebSocket                    | O sistema deve realizar tentativas automáticas de reconexão WebSocket conforme a política definida para o projeto.                              | Should     |
| RF45 | Sinalização de indisponibilidade       | O sistema web deve informar ao operador quando uma comunicação necessária para a execução estiver indisponível.                                 | Must       |
| RF46 | Comunicação MQTT                       | O sistema deve utilizar MQTT para transmissão e recepção de dados conforme a arquitetura definida para o projeto.                               | Must       |
| RF47 | Reconexão MQTT                         | O sistema deve realizar reconexão após uma perda de comunicação MQTT.                                                                           | Must       |
| RF48 | Tratamento de mensagens MQTT           | O sistema deve validar e processar as mensagens MQTT recebidas de acordo com o formato definido para a comunicação.                             | Must       |

## ÉPICO 6 — Banco de Dados e Histórico

### Área: Software

| ID   | Requisito                      | Descrição                                                                                                               | Prioridade |
| ---- | ------------------------------ | ----------------------------------------------------------------------------------------------------------------------- | ---------- |
| RF49 | Armazenamento da execução      | Após cada execução, o sistema deve armazenar os dados coletados em um banco de dados.                                   | Must       |
| RF50 | Associação ao labirinto        | Os dados armazenados devem ser associados ao labirinto correspondente à execução.                                       | Must       |
| RF51 | Consulta por labirinto         | O sistema web deve permitir a consulta dos dados de execução referentes a um labirinto específico.                      | Must       |
| RF52 | Consulta geral                 | O sistema web deve permitir a consulta conjunta dos dados armazenados de diferentes labirintos.                         | Must       |
| RF53 | Registro de falhas da execução | O sistema deve registrar falhas de comunicação, timeouts, interrupções e condições que impeçam a conclusão da execução. | Must       |

## ÉPICO 7 — Integração e Validação

### Área: Integração

| ID   | Requisito                       | Descrição                                                                                               | Prioridade |
| ---- | ------------------------------- | ------------------------------------------------------------------------------------------------------- | ---------- |
| RF54 | Correção da movimentação        | O sistema deve utilizar as informações de odometria para auxiliar no controle e posicionamento do robô. | Must       |
| RF55 | Calibração dos sensores         | O sistema deve permitir a calibração dos sensores antes da operação.                                    | Should     |
| RF56 | Sinalização do ciclo de recarga | Durante a recarga, o robô deve poder indicar visualmente o estado do ciclo de carregamento.             | Could      |

---

# Requisitos Não Funcionais (RNF)

Os **Requisitos Não Funcionais (RNF)** definem características, restrições, limites e condições de qualidade que devem ser atendidos pelo produto.

Eles descrevem **como o sistema deve funcionar**, estabelecendo características como dimensões, desempenho, autonomia, segurança, confiabilidade, compatibilidade e latência.

A prioridade segue a mesma classificação MoSCoW utilizada nos requisitos funcionais:

* **Must:** requisito indispensável para a entrega e validação do projeto.
* **Should:** requisito importante, a ser implementado quando houver viabilidade técnica e de prazo.
* **Could:** requisito desejável, implementado caso não comprometa os itens de maior prioridade.

## ÉPICO 1 — Estrutura do Micromouse

### Área: Estruturas

| ID    | Requisito                                     | Descrição                                                                                                                                                         | Prioridade |
| ----- | --------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RNF1  | Limite dimensional do chassi                  | O chassi deve possuir dimensões máximas de 16,5 × 16,5 cm e não apresentar interferências mecânicas durante a operação.                                           | Must       |
| RNF2  | Compatibilidade com as dimensões do labirinto | A geometria deve permitir movimentação livre em células de 18 cm, sem pontos cortantes, salientes ou de atrito que possam causar travamentos ou danos.            | Must       |
| RNF3  | Massa estrutural                              | A massa da estrutura deve permanecer dentro do limite estabelecido, sem comprometer aceleração, frenagem, estabilidade ou autonomia.                              | Must       |
| RNF4  | Resistência mecânica                          | A estrutura deve suportar movimentação, aceleração, frenagem e impactos sem sofrer deformações que afetem seu funcionamento.                                      | Must       |
| RNF5  | Rigidez estrutural                            | A estrutura deve possuir rigidez suficiente para preservar sua geometria e a posição dos componentes, evitando deformações ou folgas que prejudiquem a navegação. | Must       |
| RNF6  | Estabilidade estrutural                       | A estrutura deve permanecer estável durante acelerações, frenagens, curvas e mudanças de direção.                                                                 | Must       |
| RNF7  | Distribuição de massa                         | A distribuição de massa deve ser adequada, evitando desequilíbrios que prejudiquem o funcionamento do robô.                                                       | Should     |
| RNF8  | Compatibilidade entre subsistemas             | As dimensões, espaços e pontos de fixação da estrutura devem ser compatíveis com os componentes de Hardware, Energia e Software.                                  | Must       |
| RNF9  | Precisão dimensional de fabricação            | As dimensões finais da estrutura devem permanecer dentro das tolerâncias definidas para o projeto.                                                                | Must       |
| RNF10 | Segurança estrutural                          | A estrutura não deve possuir pontos, arestas ou elementos expostos que possam causar danos ou comprometer a operação.                                             | Must       |
| RNF11 | Durabilidade da estrutura                     | As características mecânicas e dimensões da estrutura devem ser mantidas durante os testes e execuções.                                                           | Should     |
| RNF12 | Facilidade de montagem e desmontagem          | A estrutura deve permitir montagem, desmontagem e manutenção utilizando as ferramentas disponíveis, sem procedimentos excessivamente complexos.                   | Should     |
| RNF13 | Aproveitamento do espaço interno              | O espaço interno deve ser utilizado de forma eficiente, sem comprometer circulação, manutenção, ventilação ou integração dos demais subsistemas.                  | Should     |
| RNF14 | Compatibilidade com os materiais disponíveis  | Os materiais utilizados devem ser compatíveis com os processos de fabricação, ferramentas, orçamento e recursos disponíveis.                                      | Must       |

## ÉPICO 2 — Hardware e Sensoriamento

### Área: Hardware

| ID    | Requisito                               | Descrição                                                                                                                                        | Prioridade |
| ----- | --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ | ---------- |
| RNF15 | Limite Dimensional e Geométrico         | O micromouse deve possuir largura máxima de 16,5 cm e comprimento máximo de 16,5 cm em qualquer estado operacional.                              | Must       |
| RNF16 | Autonomia de Alimentação                | A bateria deve fornecer autonomia mínima de 30 minutos ou permitir a realização de 3 execuções completas.                                        | Must       |
| RNF17 | Proteção e Margem de Segurança Elétrica | O sistema deve possuir proteção contra inversão de polaridade e condutores dimensionados com margem mínima de 30% em relação à corrente de pico. | Must       |
| RNF18 | Estabilidade de Tensão Lógica           | O regulador deve manter as linhas de 3,3 V e 5 V dentro de uma variação máxima de ±5% durante a partida dos motores.                             | Must       |
| RNF19 | Latência da Malha Física de Resposta    | O tempo entre a leitura de um sensor e o acionamento efetivo do motor deve ser de, no máximo, 50 ms.                                             | Must       |

## ÉPICO 3 — Alimentação e Energia

### Área: Energia

| ID    | Requisito                                 | Descrição                                                                                                                                                            | Prioridade |
| ----- | ----------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RNF20 | Autonomia operacional                     | Com sensores, controle, motores e telemetria ativos, o sistema deve operar por pelo menos 30 minutos sem recarga ou substituição da bateria.                         | Must       |
| RNF21 | Rendimento da conversão de energia        | Os conversores e reguladores devem apresentar eficiência mínima de 85% na carga nominal.                                                                             | Should     |
| RNF22 | Consumo em inatividade                    | Com o sistema ligado, mas sem execução, a corrente total deve ser de no máximo 50 mA, desabilitando periféricos desnecessários.                                      | Should     |
| RNF23 | Aviso preventivo de descarga              | O sistema deve emitir alerta quando qualquer célula atingir 3,3 V, antes de atingir uma faixa potencialmente prejudicial.                                            | Must       |
| RNF24 | Restrição de massa do conjunto energético | Os componentes relacionados à energia devem representar no máximo 25% da massa final do produto.                                                                     | Must       |
| RNF25 | Integridade da alimentação dos sensores   | Durante acelerações e partidas dos motores, a alimentação dos sensores deve permanecer entre 95% e 105% da tensão nominal.                                           | Must       |
| RNF26 | Proteção contra falhas elétricas          | O sistema deve interromper ou limitar a corrente em situações de sobrecorrente ou curto-circuito.                                                                    | Must       |
| RNF27 | Confiabilidade da telemetria energética   | Após calibração, as medições de tensão e consumo devem apresentar erro máximo de 5% em relação a um instrumento de referência.                                       | Should     |
| RNF28 | Compatibilidade de tensão dos subsistemas | A tensão nominal e a faixa de descarga da bateria devem ser compatíveis com todos os subsistemas, com os BECs dimensionados para as tensões e correntes necessárias. | Must       |

## ÉPICO 4 — Navegação e Controle

### Área: Software

| ID    | Requisito                              | Descrição                                                                                                                                       | Prioridade |
| ----- | -------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RNF29 | Tempo máximo de execução               | Cada tentativa de solução do labirinto deve ser realizada em, no máximo, 10 minutos.                                                            | Must       |
| RNF30 | Compatibilidade com os labirintos      | O software de navegação deve ser compatível com labirintos de 4 × 4, 8 × 4 e 12 × 4, com células de 18 cm.                                      | Must       |
| RNF31 | Compatibilidade com o hardware         | O software embarcado deve ser compatível com os componentes eletrônicos necessários para sensoriamento, navegação e comunicação.                | Must       |
| RNF32 | Compatibilidade com o microcontrolador | O software embarcado deve ser executável no ESP32.                                                                                              | Must       |
| RNF33 | Timeout da conexão WebSocket           | O sistema deve detectar uma conexão WebSocket inativa após o tempo máximo de timeout definido pelo projeto.                                     | Must       |
| RNF34 | Tempo de reconexão WebSocket           | O sistema deve realizar a reconexão WebSocket de acordo com o intervalo e o número máximo de tentativas definidos pelo projeto.                 | Should     |
| RNF35 | Timeout/Keep-Alive MQTT                | A comunicação MQTT deve utilizar mecanismos de keep-alive e timeout compatíveis com os requisitos de comunicação do projeto.                    | Must       |
| RNF36 | Reconexão MQTT                         | O sistema deve possuir mecanismo de reconexão após perda da comunicação MQTT.                                                                   | Must       |
| RNF37 | Integridade das mensagens              | As mensagens trocadas devem seguir o formato definido pelo projeto e possuir mecanismos para identificação e tratamento de mensagens inválidas. | Must       |
| RNF38 | Latência da telemetria                 | Os dados de telemetria devem estar disponíveis no sistema web dentro da latência máxima definida pelo projeto.                                  | Must       |
| RNF39 | Precisão de sensoriamento              | Os sensores devem possuir precisão suficiente para permitir a navegação dentro das tolerâncias definidas pelo projeto.                          | Should     |

## ÉPICO 5 — Sistema Web e Telemetria

### Área: Software

| ID    | Requisito                               | Descrição                                                                                                                                                                                       | Prioridade |
| ----- | --------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
| RNF40 | Atualização da telemetria               | O sistema web deve receber, processar e atualizar os dados de telemetria em tempo real.                                                                                                         | Must       |
| RNF41 | Integridade da telemetria               | Os dados recebidos e apresentados pelo sistema web devem corresponder aos dados transmitidos pelo robô, sem alterações indevidas.                                                               | Must       |
| RNF42 | Adaptação da representação do labirinto | O sistema web deve adaptar a representação visual do mapa às três dimensões de labirinto, mantendo todas as células e o trajeto visíveis.                                                       | Should     |
| RNF43 | Legibilidade da telemetria              | O sistema web deve apresentar separadamente os seis dados obrigatórios de telemetria: tipo de labirinto, trajeto, consumo da bateria, velocidade média, tempo de conclusão e status do desafio. | Must       |
| RNF44 | Responsividade da interface             | O sistema web deve se adaptar a diferentes tamanhos de tela sem sobreposição ou corte dos dados obrigatórios de telemetria.                                                                     | Should     |
| RNF45 | Restrição de recursos                   | As tecnologias utilizadas no software devem ser compatíveis com os recursos técnicos, materiais e financeiros disponíveis para o projeto.                                                       | Must       |

## ÉPICO 6 — Banco de Dados e Histórico

### Área: Software

| ID    | Requisito                         | Descrição                                                                                                    | Prioridade |
| ----- | --------------------------------- | ------------------------------------------------------------------------------------------------------------ | ---------- |
| RNF46 | Persistência dos dados            | Os dados de uma execução concluída devem permanecer armazenados e disponíveis para consultas posteriores.    | Must       |
| RNF47 | Integridade dos dados armazenados | Os dados armazenados devem preservar corretamente a associação entre cada execução e o respectivo labirinto. | Must       |

## ÉPICO 7 — Integração e Validação

O Épico 7 reúne requisitos relacionados à integração dos diferentes subsistemas e à validação do produto. Os requisitos não funcionais específicos dessa etapa serão definidos conforme os testes de integração e validação forem detalhados.
