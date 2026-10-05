# Backend

API do Rato Cego, inicializada pelo [Spring Initializr](https://start.spring.io/) com Java 21, Maven e Spring Boot 4.1.1.

## Requisitos

- JDK 21 ou superior
- PostgreSQL local acessível em `localhost:5432`
- Maven Wrapper incluído (`./mvnw`)

## Executar

```bash
./mvnw spring-boot:run
```

Por padrão, a aplicação usa o banco `ratocego`, usuário e senha `ratocego`. Configure `DATABASE_URL`, `DATABASE_USERNAME` e `DATABASE_PASSWORD` para usar outros valores. A configuração MQTT fica nas variáveis `MQTT_SERVER_URI`, `MQTT_CLIENT_ID` e `MQTT_TOPIC_PREFIX`.

O projeto inclui starters para REST, validação, WebSocket, JPA e Actuator; Spring Integration MQTT com cliente Eclipse Paho; driver PostgreSQL; e Spring Boot DevTools (opcional, apenas para desenvolvimento). O Maven Wrapper está incluído.

Os pacotes seguem a arquitetura em `controller`, `dto`, `model`, `repository`, `service`, `config` e `adapter` (`mqtt`, `persistence` e `websocket`). As funcionalidades ainda serão implementadas.
