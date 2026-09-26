### 1. Visão de dados e escolha do banco

Propõe-se PostgreSQL , executado localmente, como banco relacional. A aplicação consultada por labirinto e estado, mantém uma relação de um labirinto para muitas tentativas e armazena uma série ordenada de amostras por tentativa. Chaves estrangeiras e restrições de unicidade protegem essa associação e impedem que a mesma sequência de telemetria seja gravada duas vezes. A escolha é uma decisão de projeto de proposta nesta seção; requer concordância da equipe, pois a documentação anterior ainda deixa o banco indefinido. No backend, a persistência pode ser inovadora com Spring Data JPA; a escolha final da biblioteca acompanha a confirmação do banco.
O banco guarda histórico e estado do sistema web . O mapa usado para decidir movimentos permanece sob responsabilidade do robô. Com o contrato MQTT atual, o backend recebe posição e orientação por amostra, suficiente para reconstruir a trajetória observada ; ele não obtém a topologia completa das paredes. Se uma interface precisar mostrar paredes descobertas, uma equipe de embarcados e um software terão que definir um evento de mapeamento e sua persistência antes de prometer essa visualização.


### 1.1 MER — modelo entidade relacionamento conceitual
Labirinto identifica uma configuração nominal do desafio, com tipo e dimensões. Tentativa representa uma execução única e pertence exatamente a um labirinto; um labirinto pode ter vários esforços. Amostra de telemetria pertence exatamente a uma tentativa; uma tentativa pode não ter amostras, por exemplo, se o início. A posição é um conjunto de atributos da amostra, consistente com o objeto. Posição do diagrama de classes, sem necessidade de tabela independente.

{tabela:}

### 2. Visão de processos
O backend concentra-se nas regras do ciclo de vida. O robô faz a leitura dos sensores, o mapeamento, a localização e a navegação sem comandos de movimento enviados pelo frontend. A interface solicita apenas o início e a interrupção e acompanha os resultados.

| Processo | Entrada | Responsável | Saída e persistência |
|---|---|---|---|
| Solicitar início | `POST /api/runs` com `mazeType` | `RunController` e `RunService` | Cria `run` em `START_REQUESTED`, gera `runId` e publica `run.start` |
| Confirmar início | MQTT `run.started` | `TelemetryMqttListener` e `RunService` | Confere tipo/dimensões, marca `IN_PROGRESS` e envia `RUN_STARTED` |
| Tratar telemetria | MQTT `telemetry.sample` | `TelemetryService` | Valida `runId` e `sequence`, calcula indicadores, grava amostra e envia `TELEMETRY_UPDATE` |
| Solicitar interrupção | `POST /api/runs/{runId}/interrupt` | `RunService` | Marca `INTERRUPT_REQUESTED`, publica `run.interrupt` e informa estado pendente |
| Encerrar tentativa | MQTT `run.finished` ou `run.interrupted` | `RunService` | Grava resumo e estado terminal; envia `RUN_FINISHED` ou `RUN_INTERRUPTED` |
| Consultar histórico | GET da API REST | `RunController`, `RunService` e repositórios | Lista geral, filtro por labirinto e detalhe com trajeto |

(imagem)

