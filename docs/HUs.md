# Histórias de Usuário

Este documento apresenta as **Histórias de Usuário (HUs)** do projeto **Micromouse**, elaboradas a partir dos Requisitos Funcionais (RFs) definidos para o sistema.

As Histórias de Usuário representam as funcionalidades do projeto sob a perspectiva dos usuários, operadores, integrantes da equipe de desenvolvimento ou dos próprios componentes do sistema.

Cada Requisito Funcional possui uma História de Usuário correspondente. Os Requisitos Não Funcionais não possuem HUs próprias, pois representam restrições, características de qualidade e condições que devem ser atendidas pelas funcionalidades do sistema.

Os **critérios de aceitação** de cada História de Usuário serão definidos posteriormente nas respectivas **Issues** do projeto.

---

## Organização das Histórias de Usuário

As HUs estão organizadas de acordo com os sete épicos definidos para o projeto:

| Épico | Descrição | HUs |
|---|---|---|
| **ÉPICO 1** | Estrutura do Micromouse | HU01–HU08 |
| **ÉPICO 2** | Hardware e Sensoriamento | HU09–HU16 |
| **ÉPICO 3** | Alimentação e Energia | HU17–HU20 |
| **ÉPICO 4** | Navegação e Controle | HU21–HU35 |
| **ÉPICO 5** | Sistema Web e Telemetria | HU36–HU48 |
| **ÉPICO 6** | Banco de Dados e Histórico | HU49–HU53 |
| **ÉPICO 7** | Integração e Validação | HU54–HU56 |

### Prioridades

As prioridades utilizadas nas histórias são:

- **Must** — funcionalidade essencial para o funcionamento do sistema.
- **Should** — funcionalidade importante, mas que pode ser implementada posteriormente caso necessário.
- **Could** — funcionalidade desejável, mas de menor prioridade.

---

# ÉPICO 1 — Estrutura do Micromouse

Este épico reúne as histórias relacionadas à construção física do Micromouse, incluindo proteção, fixação, modularidade, organização dos componentes, cabeamento e acesso aos elementos internos e externos.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu01"></a>**HU01** | **Como** integrante da equipe de desenvolvimento, **quero** que a estrutura do Micromouse proteja seus componentes internos, **para** evitar danos aos componentes eletrônicos durante a operação e possíveis colisões no labirinto. | [RF1](Requisitos.md#rf1) | **Must** | [RNF1](Requisitos.md#rnf1), [RNF4](Requisitos.md#rnf4), [RNF5](Requisitos.md#rnf5), [RNF10](Requisitos.md#rnf10) |
| <a id="hu02"></a>**HU02** | **Como** integrante da equipe de desenvolvimento, **quero** ter acesso aos componentes internos do Micromouse, **para** realizar inspeções, manutenção, substituições e ajustes sem precisar desmontar completamente o chassi. | [RF2](Requisitos.md#rf2) | **Should** | [RNF12](Requisitos.md#rnf12), [RNF13](Requisitos.md#rnf13) |
| <a id="hu03"></a>**HU03** | **Como** integrante da equipe de desenvolvimento, **quero** que os subsistemas possuam pontos adequados de fixação, **para** evitar deslocamentos dos componentes durante o funcionamento. | [RF3](Requisitos.md#rf3) | **Must** | [RNF5](Requisitos.md#rnf5), [RNF6](Requisitos.md#rnf6), [RNF8](Requisitos.md#rnf8) |
| <a id="hu04"></a>**HU04** | **Como** integrante da equipe de desenvolvimento, **quero** que a estrutura seja modular, **para** permitir a substituição ou atualização de módulos sem reconstruir completamente o chassi. | [RF4](Requisitos.md#rf4) | **Should** | [RNF8](Requisitos.md#rnf8), [RNF12](Requisitos.md#rnf12), [RNF13](Requisitos.md#rnf13) |
| <a id="hu05"></a>**HU05** | **Como** integrante da equipe de desenvolvimento, **quero** que motores, rodas e demais elementos de tração sejam corretamente fixados e alinhados, **para** garantir uma movimentação estável e previsível. | [RF5](Requisitos.md#rf5) | **Must** | [RNF5](Requisitos.md#rnf5), [RNF6](Requisitos.md#rnf6), [RNF7](Requisitos.md#rnf7) |
| <a id="hu06"></a>**HU06** | **Como** integrante da equipe de desenvolvimento, **quero** que os sensores possuam espaços e pontos de fixação adequados, **para** manter seu posicionamento e orientação corretos durante a navegação. | [RF6](Requisitos.md#rf6) | **Must** | [RNF8](Requisitos.md#rnf8), [RNF13](Requisitos.md#rnf13), [RNF37](Requisitos.md#rnf37) |
| <a id="hu07"></a>**HU07** | **Como** integrante da equipe de desenvolvimento, **quero** que os cabos sejam organizados e protegidos, **para** evitar interferências com rodas, motores, sensores e demais componentes. | [RF7](Requisitos.md#rf7) | **Must** | [RNF8](Requisitos.md#rnf8), [RNF10](Requisitos.md#rnf10), [RNF15](Requisitos.md#rnf15) |
| <a id="hu08"></a>**HU08** | **Como** operador, **quero** ter acesso aos interruptores, conectores e interfaces externas, **para** operar e realizar a manutenção do Micromouse sem precisar desmontar sua estrutura. | [RF8](Requisitos.md#rf8) | **Should** | [RNF10](Requisitos.md#rnf10), [RNF12](Requisitos.md#rnf12) |

---

# ÉPICO 2 — Hardware e Sensoriamento

Este épico reúne as histórias relacionadas aos componentes eletrônicos responsáveis pela percepção do ambiente, processamento embarcado, acionamento dos motores, comunicação, alimentação e interface física.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu09"></a>**HU09** | **Como** Micromouse, **quero** realizar leituras contínuas dos sensores para identificar paredes e aberturas ao meu redor, **para** obter informações do ambiente e orientar minha navegação. | [RF9](Requisitos.md#rf9) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF28](Requisitos.md#rnf28), [RNF37](Requisitos.md#rnf37) |
| <a id="hu10"></a>**HU10** | **Como** Micromouse, **quero** processar localmente as informações dos sensores e executar a lógica de controle e navegação, **para** funcionar de forma autônoma sem depender de processamento externo. | [RF10](Requisitos.md#rf10) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29), [RNF30](Requisitos.md#rnf30) |
| <a id="hu11"></a>**HU11** | **Como** sistema de controle do Micromouse, **quero** enviar comandos para acionar os motores nos dois sentidos e controlar sua velocidade, **para** executar os movimentos necessários durante a navegação. | [RF11](Requisitos.md#rf11) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF23](Requisitos.md#rnf23), [RNF26](Requisitos.md#rnf26) |
| <a id="hu12"></a>**HU12** | **Como** Micromouse, **quero** controlar diferencialmente os motores durante o deslocamento, **para** manter uma trajetória reta e centralizada entre as paredes. | [RF12](Requisitos.md#rf12) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF37](Requisitos.md#rnf37) |
| <a id="hu13"></a>**HU13** | **Como** Micromouse, **quero** executar curvas e rotações de 90° e 180°, **para** mudar de direção e percorrer diferentes caminhos do labirinto. | [RF13](Requisitos.md#rf13) | **Must** | [RNF2](Requisitos.md#rnf2), [RNF28](Requisitos.md#rnf28), [RNF35](Requisitos.md#rnf35) |
| <a id="hu14"></a>**HU14** | **Como** operador, **quero** receber informações de telemetria durante a execução por meio de comunicação sem fio, **para** acompanhar o funcionamento do Micromouse durante o desafio. | [RF14](Requisitos.md#rf14) | **Must** | [RNF31](Requisitos.md#rnf31), [RNF33](Requisitos.md#rnf33), [RNF36](Requisitos.md#rnf36), [RNF39](Requisitos.md#rnf39) |
| <a id="hu15"></a>**HU15** | **Como** integrante da equipe de desenvolvimento, **quero** que a fonte de energia forneça tensões reguladas e estáveis, **para** garantir o funcionamento adequado dos componentes eletrônicos, sensores e atuadores. | [RF15](Requisitos.md#rf15) | **Must** | [RNF15](Requisitos.md#rnf15), [RNF16](Requisitos.md#rnf16), [RNF23](Requisitos.md#rnf23), [RNF26](Requisitos.md#rnf26) |
| <a id="hu16"></a>**HU16** | **Como** operador, **quero** utilizar uma interface física externa para iniciar e interromper a execução, **para** controlar o início e a parada do desafio. | [RF16](Requisitos.md#rf16) | **Must** | [RNF10](Requisitos.md#rnf10) |

---

# ÉPICO 3 — Alimentação e Energia

Este épico reúne as histórias relacionadas ao fornecimento, monitoramento, isolamento e manutenção da alimentação elétrica do Micromouse.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu17"></a>**HU17** | **Como** operador, **quero** acompanhar o consumo e a condição da bateria durante a execução, **para** monitorar o estado energético do Micromouse. | [RF17](Requisitos.md#rf17) | **Must** | [RNF18](Requisitos.md#rnf18), [RNF21](Requisitos.md#rnf21), [RNF25](Requisitos.md#rnf25) |
| <a id="hu18"></a>**HU18** | **Como** integrante da equipe de desenvolvimento, **quero** que um interruptor físico acessível desconecte a bateria dos subsistemas, **para** garantir segurança durante transporte, montagem e manutenção. | [RF18](Requisitos.md#rf18) | **Must** | [RNF15](Requisitos.md#rnf15), [RNF24](Requisitos.md#rnf24) |
| <a id="hu19"></a>**HU19** | **Como** operador, **quero** receber um aviso na interface web quando a bateria atingir um nível crítico, **para** saber quando a alimentação estiver próxima de uma condição inadequada. | [RF19](Requisitos.md#rf19) | **Must** | [RNF21](Requisitos.md#rnf21), [RNF36](Requisitos.md#rnf36), [RNF38](Requisitos.md#rnf38) |
| <a id="hu20"></a>**HU20** | **Como** integrante da equipe de desenvolvimento, **quero** remover e reinstalar a bateria sem desmontar o chassi, **para** facilitar sua manutenção e substituição. | [RF20](Requisitos.md#rf20) | **Could** | [RNF12](Requisitos.md#rnf12), [RNF22](Requisitos.md#rnf22), [RNF26](Requisitos.md#rnf26) |

---

# ÉPICO 4 — Navegação e Controle

Este épico reúne as histórias relacionadas à percepção do labirinto, localização, planejamento e execução do percurso, gerenciamento da execução e controle da movimentação.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu21"></a>**HU21** | **Como** Micromouse, **quero** identificar paredes por meio dos sensores, **para** conhecer as características do caminho ao meu redor. | [RF21](Requisitos.md#rf21) | **Must** | [RNF28](Requisitos.md#rnf28), [RNF37](Requisitos.md#rnf37) |
| <a id="hu22"></a>**HU22** | **Como** Micromouse, **quero** acompanhar minha posição no labirinto, **para** utilizar essa informação durante a navegação e o registro do percurso. | [RF22](Requisitos.md#rf22) | **Must** | [RNF28](Requisitos.md#rnf28), [RNF35](Requisitos.md#rnf35) |
| <a id="hu23"></a>**HU23** | **Como** Micromouse, **quero** determinar autonomamente meus movimentos com base nas informações obtidas do ambiente, **para** escolher e seguir um percurso no labirinto. | [RF23](Requisitos.md#rf23) | **Must** | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28), [RNF30](Requisitos.md#rnf30) |
| <a id="hu24"></a>**HU24** | **Como** operador, **quero** que o Micromouse navegue sem intervenção humana durante a execução, **para** realizar o desafio de forma autônoma. | [RF24](Requisitos.md#rf24) | **Must** | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28), [RNF29](Requisitos.md#rnf29), [RNF30](Requisitos.md#rnf30) |
| <a id="hu25"></a>**HU25** | **Como** Micromouse, **quero** identificar quando alcançar a região objetivo, **para** determinar a conclusão do percurso. | [RF25](Requisitos.md#rf25) | **Must** | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28) |
| <a id="hu26"></a>**HU26** | **Como** operador, **quero** que o trajeto seja registrado em cada execução, **para** acompanhar e consultar posteriormente o caminho percorrido. | [RF26](Requisitos.md#rf26) | **Must** | [RNF39](Requisitos.md#rnf39), [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu27"></a>**HU27** | **Como** operador, **quero** selecionar o tipo de labirinto antes da execução, **para** configurar o sistema de acordo com o desafio a ser realizado. | [RF27](Requisitos.md#rf27) | **Must** | [RNF28](Requisitos.md#rnf28), [RNF40](Requisitos.md#rnf40) |
| <a id="hu28"></a>**HU28** | **Como** operador, **quero** que o sistema verifique as condições necessárias antes de iniciar a execução, **para** evitar o início do percurso quando alguma condição obrigatória estiver indisponível. | [RF28](Requisitos.md#rf28) | **Must** | [RNF18](Requisitos.md#rnf18), [RNF26](Requisitos.md#rnf26), [RNF28](Requisitos.md#rnf28) |
| <a id="hu29"></a>**HU29** | **Como** operador, **quero** iniciar o percurso após a preparação do sistema, **para** colocar o Micromouse em funcionamento no labirinto selecionado. | [RF29](Requisitos.md#rf29) | **Must** | [RNF27](Requisitos.md#rnf27) |
| <a id="hu30"></a>**HU30** | **Como** operador, **quero** interromper a execução por meio da interface disponível, **para** parar o desafio quando necessário. | [RF30](Requisitos.md#rf30) | **Must** | [RNF24](Requisitos.md#rnf24) |
| <a id="hu31"></a>**HU31** | **Como** operador, **quero** iniciar uma nova execução após a conclusão ou interrupção de uma tentativa, **para** realizar novas tentativas sem precisar reinicializar manualmente todo o sistema. | [RF31](Requisitos.md#rf31) | **Should** | [RNF27](Requisitos.md#rnf27), [RNF44](Requisitos.md#rnf44) |
| <a id="hu32"></a>**HU32** | **Como** operador, **quero** que o sistema mantenha e disponibilize o estado atual da execução, **para** saber se o Micromouse está aguardando, executando, concluído, interrompido ou em situação de falha ou timeout. | [RF32](Requisitos.md#rf32) | **Must** | [RNF35](Requisitos.md#rnf35), [RNF39](Requisitos.md#rnf39) |
| <a id="hu33"></a>**HU33** | **Como** sistema de controle, **quero** encerrar automaticamente a execução quando o objetivo for alcançado, o tempo máximo for excedido ou ocorrer uma falha segura, **para** garantir que cada tentativa tenha um encerramento definido. | [RF33](Requisitos.md#rf33) | **Must** | [RNF27](Requisitos.md#rnf27) |
| <a id="hu34"></a>**HU34** | **Como** sistema de controle, **quero** interromper ou limitar o acionamento dos motores de forma segura quando ocorrer uma interrupção ou falha, **para** evitar movimentos inadequados e proteger o Micromouse. | [RF34](Requisitos.md#rf34) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF24](Requisitos.md#rnf24) |
| <a id="hu35"></a>**HU35** | **Como** Micromouse, **quero** utilizar encoders para medir deslocamento e velocidade, **para** auxiliar o controle e a navegação. | [RF35](Requisitos.md#rf35) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29) |

---

# ÉPICO 5 — Sistema Web e Telemetria

Este épico reúne as histórias relacionadas à comunicação entre o Micromouse e a aplicação web, transmissão de telemetria, monitoramento da execução, WebSocket e MQTT.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu36"></a>**HU36** | **Como** operador, **quero** que a aplicação web receba e disponibilize a telemetria enviada pelo Micromouse, **para** acompanhar a execução em tempo real. | [RF36](Requisitos.md#rf36) | **Must** | [RNF36](Requisitos.md#rnf36), [RNF38](Requisitos.md#rnf38), [RNF39](Requisitos.md#rnf39) |
| <a id="hu37"></a>**HU37** | **Como** operador, **quero** visualizar na aplicação web o trajeto realizado pelo Micromouse, **para** acompanhar seu deslocamento durante a execução. | [RF37](Requisitos.md#rf37) | **Must** | [RNF38](Requisitos.md#rnf38), [RNF40](Requisitos.md#rnf40), [RNF41](Requisitos.md#rnf41) |
| <a id="hu38"></a>**HU38** | **Como** operador, **quero** visualizar na aplicação web a condição e o consumo da bateria, **para** acompanhar o estado energético do Micromouse. | [RF38](Requisitos.md#rf38) | **Must** | [RNF21](Requisitos.md#rnf21), [RNF25](Requisitos.md#rnf25), [RNF38](Requisitos.md#rnf38), [RNF41](Requisitos.md#rnf41) |
| <a id="hu39"></a>**HU39** | **Como** operador, **quero** acompanhar o tempo de execução na aplicação web, **para** saber a duração do percurso realizado. | [RF39](Requisitos.md#rf39) | **Must** | [RNF27](Requisitos.md#rnf27), [RNF38](Requisitos.md#rnf38), [RNF41](Requisitos.md#rnf41) |
| <a id="hu40"></a>**HU40** | **Como** operador, **quero** visualizar a velocidade média na aplicação web, **para** acompanhar o desempenho do Micromouse durante a execução. | [RF40](Requisitos.md#rf40) | **Must** | [RNF38](Requisitos.md#rnf38), [RNF39](Requisitos.md#rnf39), [RNF41](Requisitos.md#rnf41) |
| <a id="hu41"></a>**HU41** | **Como** operador, **quero** visualizar o resultado do desafio na aplicação web, **para** saber se o percurso foi concluído. | [RF41](Requisitos.md#rf41) | **Must** | [RNF41](Requisitos.md#rnf41), [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu42"></a>**HU42** | **Como** aplicação web, **quero** estabelecer uma conexão WebSocket com o serviço de comunicação em tempo real, **para** receber e disponibilizar continuamente as informações da execução. | [RF42](Requisitos.md#rf42) | **Must** | [RNF31](Requisitos.md#rnf31), [RNF36](Requisitos.md#rnf36) |
| <a id="hu43"></a>**HU43** | **Como** aplicação web, **quero** detectar a indisponibilidade da conexão WebSocket durante a execução, **para** identificar a ausência de dados em tempo real. | [RF43](Requisitos.md#rf43) | **Must** | [RNF31](Requisitos.md#rnf31), [RNF36](Requisitos.md#rnf36) |
| <a id="hu44"></a>**HU44** | **Como** aplicação web, **quero** tentar restabelecer automaticamente a conexão WebSocket após uma perda de conexão, **para** recuperar a comunicação sem necessidade de intervenção manual imediata. | [RF44](Requisitos.md#rf44) | **Should** | [RNF32](Requisitos.md#rnf32) |
| <a id="hu45"></a>**HU45** | **Como** operador, **quero** ser informado quando uma comunicação necessária estiver indisponível, **para** saber que os dados podem estar temporariamente indisponíveis. | [RF45](Requisitos.md#rf45) | **Must** | [RNF31](Requisitos.md#rnf31), [RNF32](Requisitos.md#rnf32), [RNF36](Requisitos.md#rnf36) |
| <a id="hu46"></a>**HU46** | **Como** sistema de comunicação, **quero** utilizar MQTT para as mensagens definidas na arquitetura, **para** realizar a troca estruturada de informações entre os componentes. | [RF46](Requisitos.md#rf46) | **Must** | [RNF33](Requisitos.md#rnf33), [RNF35](Requisitos.md#rnf35) |
| <a id="hu47"></a>**HU47** | **Como** sistema de comunicação, **quero** tentar restabelecer automaticamente a conexão MQTT após uma perda de conexão, **para** recuperar a troca de informações entre os componentes. | [RF47](Requisitos.md#rf47) | **Must** | [RNF33](Requisitos.md#rnf33), [RNF34](Requisitos.md#rnf34) |
| <a id="hu48"></a>**HU48** | **Como** sistema de comunicação, **quero** receber, validar e processar mensagens MQTT conforme o formato definido, **para** garantir que somente mensagens válidas sejam utilizadas pelo sistema. | [RF48](Requisitos.md#rf48) | **Must** | [RNF35](Requisitos.md#rnf35) |

---

# ÉPICO 6 — Banco de Dados e Histórico

Este épico reúne as histórias relacionadas ao armazenamento, organização e consulta dos dados gerados durante as execuções do Micromouse.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu49"></a>**HU49** | **Como** operador, **quero** que os dados de cada execução sejam armazenados após o percurso, **para** manter um histórico das execuções realizadas. | [RF49](Requisitos.md#rf49) | **Must** | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu50"></a>**HU50** | **Como** operador, **quero** que cada execução armazenada seja associada ao tipo de labirinto utilizado, **para** identificar a configuração correspondente à execução. | [RF50](Requisitos.md#rf50) | **Must** | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu51"></a>**HU51** | **Como** operador, **quero** consultar as execuções de determinado tipo de labirinto, **para** analisar o histórico daquela configuração. | [RF51](Requisitos.md#rf51) | **Must** | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu52"></a>**HU52** | **Como** operador, **quero** consultar os dados de diferentes labirintos e execuções, **para** acessar o histórico geral do sistema. | [RF52](Requisitos.md#rf52) | **Must** | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| <a id="hu53"></a>**HU53** | **Como** operador, **quero** que eventos relevantes que impeçam ou interrompam uma execução sejam registrados, **para** identificar as ocorrências que afetaram o percurso. | [RF53](Requisitos.md#rf53) | **Must** | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |

---

# ÉPICO 7 — Integração e Validação

Este épico reúne as histórias relacionadas à integração dos subsistemas e aos procedimentos necessários para garantir o funcionamento adequado do Micromouse.

| HU | História de Usuário | RF relacionado | Prioridade | RNFs relacionados |
|---|---|---|---|---|
| <a id="hu54"></a>**HU54** | **Como** Micromouse, **quero** utilizar os deslocamentos obtidos pelos encoders para realizar o controle e o posicionamento, **para** corrigir desvios e melhorar a precisão da movimentação. | [RF54](Requisitos.md#rf54) | **Must** | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29) |
| <a id="hu55"></a>**HU55** | **Como** integrante da equipe de desenvolvimento, **quero** calibrar os sensores antes da operação conforme o procedimento definido, **para** garantir leituras adequadas durante a navegação. | [RF55](Requisitos.md#rf55) | **Should** | [RNF37](Requisitos.md#rnf37) |
| <a id="hu56"></a>**HU56** | **Como** operador, **quero** que o Micromouse indique visualmente o estado do ciclo de recarga, **para** acompanhar o processo de carregamento. | [RF56](Requisitos.md#rf56) | **Could** | [RNF18](Requisitos.md#rnf18) |

---

# Matriz de Rastreabilidade

A matriz abaixo apresenta a relação entre cada História de Usuário, seu respectivo Requisito Funcional e os Requisitos Não Funcionais associados.

| HU | RF | RNFs |
|---|---|---|
| [HU01](#hu01) | [RF1](Requisitos.md#rf1) | [RNF1](Requisitos.md#rnf1), [RNF4](Requisitos.md#rnf4), [RNF5](Requisitos.md#rnf5), [RNF10](Requisitos.md#rnf10) |
| [HU02](#hu02) | [RF2](Requisitos.md#rf2) | [RNF12](Requisitos.md#rnf12), [RNF13](Requisitos.md#rnf13) |
| [HU03](#hu03) | [RF3](Requisitos.md#rf3) | [RNF5](Requisitos.md#rnf5), [RNF6](Requisitos.md#rnf6), [RNF8](Requisitos.md#rnf8) |
| [HU04](#hu04) | [RF4](Requisitos.md#rf4) | [RNF8](Requisitos.md#rnf8), [RNF12](Requisitos.md#rnf12), [RNF13](Requisitos.md#rnf13) |
| [HU05](#hu05) | [RF5](Requisitos.md#rf5) | [RNF5](Requisitos.md#rnf5), [RNF6](Requisitos.md#rnf6), [RNF7](Requisitos.md#rnf7) |
| [HU06](#hu06) | [RF6](Requisitos.md#rf6) | [RNF8](Requisitos.md#rnf8), [RNF13](Requisitos.md#rnf13), [RNF37](Requisitos.md#rnf37) |
| [HU07](#hu07) | [RF7](Requisitos.md#rf7) | [RNF8](Requisitos.md#rnf8), [RNF10](Requisitos.md#rnf10), [RNF15](Requisitos.md#rnf15) |
| [HU08](#hu08) | [RF8](Requisitos.md#rf8) | [RNF10](Requisitos.md#rnf10), [RNF12](Requisitos.md#rnf12) |
| [HU09](#hu09) | [RF9](Requisitos.md#rf9) | [RNF17](Requisitos.md#rnf17), [RNF28](Requisitos.md#rnf28), [RNF37](Requisitos.md#rnf37) |
| [HU10](#hu10) | [RF10](Requisitos.md#rf10) | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29), [RNF30](Requisitos.md#rnf30) |
| [HU11](#hu11) | [RF11](Requisitos.md#rf11) | [RNF17](Requisitos.md#rnf17), [RNF23](Requisitos.md#rnf23), [RNF26](Requisitos.md#rnf26) |
| [HU12](#hu12) | [RF12](Requisitos.md#rf12) | [RNF17](Requisitos.md#rnf17), [RNF37](Requisitos.md#rnf37) |
| [HU13](#hu13) | [RF13](Requisitos.md#rf13) | [RNF2](Requisitos.md#rnf2), [RNF28](Requisitos.md#rnf28), [RNF35](Requisitos.md#rnf35) |
| [HU14](#hu14) | [RF14](Requisitos.md#rf14) | [RNF31](Requisitos.md#rnf31), [RNF33](Requisitos.md#rnf33), [RNF36](Requisitos.md#rnf36), [RNF39](Requisitos.md#rnf39) |
| [HU15](#hu15) | [RF15](Requisitos.md#rf15) | [RNF15](Requisitos.md#rnf15), [RNF16](Requisitos.md#rnf16), [RNF23](Requisitos.md#rnf23), [RNF26](Requisitos.md#rnf26) |
| [HU16](#hu16) | [RF16](Requisitos.md#rf16) | [RNF10](Requisitos.md#rnf10) |
| [HU17](#hu17) | [RF17](Requisitos.md#rf17) | [RNF18](Requisitos.md#rnf18), [RNF21](Requisitos.md#rnf21), [RNF25](Requisitos.md#rnf25) |
| [HU18](#hu18) | [RF18](Requisitos.md#rf18) | [RNF15](Requisitos.md#rnf15), [RNF24](Requisitos.md#rnf24) |
| [HU19](#hu19) | [RF19](Requisitos.md#rf19) | [RNF21](Requisitos.md#rnf21), [RNF36](Requisitos.md#rnf36), [RNF38](Requisitos.md#rnf38) |
| [HU20](#hu20) | [RF20](Requisitos.md#rf20) | [RNF12](Requisitos.md#rnf12), [RNF22](Requisitos.md#rnf22), [RNF26](Requisitos.md#rnf26) |
| [HU21](#hu21) | [RF21](Requisitos.md#rf21) | [RNF28](Requisitos.md#rnf28), [RNF37](Requisitos.md#rnf37) |
| [HU22](#hu22) | [RF22](Requisitos.md#rf22) | [RNF28](Requisitos.md#rnf28), [RNF35](Requisitos.md#rnf35) |
| [HU23](#hu23) | [RF23](Requisitos.md#rf23) | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28), [RNF30](Requisitos.md#rnf30) |
| [HU24](#hu24) | [RF24](Requisitos.md#rf24) | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28), [RNF29](Requisitos.md#rnf29), [RNF30](Requisitos.md#rnf30) |
| [HU25](#hu25) | [RF25](Requisitos.md#rf25) | [RNF27](Requisitos.md#rnf27), [RNF28](Requisitos.md#rnf28) |
| [HU26](#hu26) | [RF26](Requisitos.md#rf26) | [RNF39](Requisitos.md#rnf39), [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU27](#hu27) | [RF27](Requisitos.md#rf27) | [RNF28](Requisitos.md#rnf28), [RNF40](Requisitos.md#rnf40) |
| [HU28](#hu28) | [RF28](Requisitos.md#rf28) | [RNF18](Requisitos.md#rnf18), [RNF26](Requisitos.md#rnf26), [RNF28](Requisitos.md#rnf28) |
| [HU29](#hu29) | [RF29](Requisitos.md#rf29) | [RNF27](Requisitos.md#rnf27) |
| [HU30](#hu30) | [RF30](Requisitos.md#rf30) | [RNF24](Requisitos.md#rnf24) |
| [HU31](#hu31) | [RF31](Requisitos.md#rf31) | [RNF27](Requisitos.md#rnf27), [RNF44](Requisitos.md#rnf44) |
| [HU32](#hu32) | [RF32](Requisitos.md#rf32) | [RNF35](Requisitos.md#rnf35), [RNF39](Requisitos.md#rnf39) |
| [HU33](#hu33) | [RF33](Requisitos.md#rf33) | [RNF27](Requisitos.md#rnf27) |
| [HU34](#hu34) | [RF34](Requisitos.md#rf34) | [RNF17](Requisitos.md#rnf17), [RNF24](Requisitos.md#rnf24) |
| [HU35](#hu35) | [RF35](Requisitos.md#rf35) | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29) |
| [HU36](#hu36) | [RF36](Requisitos.md#rf36) | [RNF36](Requisitos.md#rnf36), [RNF38](Requisitos.md#rnf38), [RNF39](Requisitos.md#rnf39) |
| [HU37](#hu37) | [RF37](Requisitos.md#rf37) | [RNF38](Requisitos.md#rnf38), [RNF40](Requisitos.md#rnf40), [RNF41](Requisitos.md#rnf41) |
| [HU38](#hu38) | [RF38](Requisitos.md#rf38) | [RNF21](Requisitos.md#rnf21), [RNF25](Requisitos.md#rnf25), [RNF38](Requisitos.md#rnf38), [RNF41](Requisitos.md#rnf41) |
| [HU39](#hu39) | [RF39](Requisitos.md#rf39) | [RNF27](Requisitos.md#rnf27), [RNF38](Requisitos.md#rnf38), [RNF41](Requisitos.md#rnf41) |
| [HU40](#hu40) | [RF40](Requisitos.md#rf40) | [RNF38](Requisitos.md#rnf38), [RNF39](Requisitos.md#rnf39), [RNF41](Requisitos.md#rnf41) |
| [HU41](#hu41) | [RF41](Requisitos.md#rf41) | [RNF41](Requisitos.md#rnf41), [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU42](#hu42) | [RF42](Requisitos.md#rf42) | [RNF31](Requisitos.md#rnf31), [RNF36](Requisitos.md#rnf36) |
| [HU43](#hu43) | [RF43](Requisitos.md#rf43) | [RNF31](Requisitos.md#rnf31), [RNF36](Requisitos.md#rnf36) |
| [HU44](#hu44) | [RF44](Requisitos.md#rf44) | [RNF32](Requisitos.md#rnf32) |
| [HU45](#hu45) | [RF45](Requisitos.md#rf45) | [RNF31](Requisitos.md#rnf31), [RNF32](Requisitos.md#rnf32), [RNF36](Requisitos.md#rnf36) |
| [HU46](#hu46) | [RF46](Requisitos.md#rf46) | [RNF33](Requisitos.md#rnf33), [RNF35](Requisitos.md#rnf35) |
| [HU47](#hu47) | [RF47](Requisitos.md#rf47) | [RNF33](Requisitos.md#rnf33), [RNF34](Requisitos.md#rnf34) |
| [HU48](#hu48) | [RF48](Requisitos.md#rf48) | [RNF35](Requisitos.md#rnf35) |
| [HU49](#hu49) | [RF49](Requisitos.md#rf49) | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU50](#hu50) | [RF50](Requisitos.md#rf50) | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU51](#hu51) | [RF51](Requisitos.md#rf51) | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU52](#hu52) | [RF52](Requisitos.md#rf52) | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU53](#hu53) | [RF53](Requisitos.md#rf53) | [RNF44](Requisitos.md#rnf44), [RNF45](Requisitos.md#rnf45) |
| [HU54](#hu54) | [RF54](Requisitos.md#rf54) | [RNF17](Requisitos.md#rnf17), [RNF29](Requisitos.md#rnf29) |
| [HU55](#hu55) | [RF55](Requisitos.md#rf55) | [RNF37](Requisitos.md#rnf37) |
| [HU56](#hu56) | [RF56](Requisitos.md#rf56) | [RNF18](Requisitos.md#rnf18) |

---

