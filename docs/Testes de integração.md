
https://github.com/user-attachments/assets/0e80e685-bf73-49c9-a106-cb3cf6c5a80b

# Testes de Integração – Hardware
O presente documento tem como objetivo apresentar e documentar o desenvolvimento e a validação da parte de hardware do robô Micromouse (Rato Cego), desenvolvido no âmbito da disciplina de Projeto Integrador 1 (PI1). Serão abordados os procedimentos de testes realizados durante o processo de desenvolvimento, visando verificar o funcionamento individual dos componentes e, principalmente, a integração entre os diferentes sistemas que compõem o robô.

Ao longo deste documento, serão apresentados os testes relacionados aos sensores, motores, encoders, circuitos de alimentação e amostragem da tensão do barramento, além dos ensaios de locomoção e movimentação do robô. Esses testes têm como finalidade avaliar o comportamento dos componentes em condições reais de operação, identificar possíveis falhas e inconsistências e garantir que os sistemas estejam funcionando conforme o esperado.

Além da verificação individual dos componentes, os testes de integração buscam assegurar a comunicação e o funcionamento conjunto entre os módulos de hardware, permitindo analisar a resposta do robô aos comandos recebidos e a confiabilidade das informações coletadas pelos sensores e demais dispositivos.

Dessa forma, esta etapa representa uma parte fundamental do desenvolvimento do projeto, uma vez que permite validar as soluções implementadas, realizar os ajustes necessários e minimizar possíveis problemas durante as etapas posteriores de operação e navegação autônoma do Micromouse. O objetivo final é garantir uma plataforma de hardware funcional, estável e confiável, capaz de atender aos requisitos estabelecidos para o funcionamento do robô.

## Testes de Bateria
A seguir, é apresentado o teste de monitoramento da tensão da bateria do robô, realizado por meio do módulo INA219. Durante o ensaio, os valores medidos pelo sensor foram enviados e exibidos no Monitor Serial da Arduino IDE, permitindo acompanhar as leituras em tempo real e verificar o funcionamento do sistema de aquisição e monitoramento da tensão de alimentação.




https://github.com/user-attachments/assets/f4f2afd3-7a65-4fcf-b789-e954044bd3dd



## Testes dos Motores

A seguir, foram realizados os testes individuais de ambos os motores responsáveis pela locomoção do robô Micromouse, com o objetivo de verificar seu funcionamento, sentido de rotação e resposta aos comandos enviados pelo sistema de controle. Durante os ensaios, foram avaliados os acionamentos dos motores em ambos os sentidos, permitindo identificar possíveis inconsistências no funcionamento e garantir que os dois conjuntos apresentassem respostas adequadas aos comandos estabelecidos.

Além disso, os testes permitiram verificar a integração entre os motores e o circuito responsável pelo seu acionamento, assegurando que os sinais de controle fossem interpretados corretamente e convertidos em movimento mecânico. Essa etapa foi fundamental para validar o sistema de tração do robô, uma vez que o funcionamento adequado e sincronizado dos dois motores é indispensável para a realização de movimentos em linha reta, curvas e manobras necessárias durante a navegação pelo labirinto.

### Motor esquerdo


https://github.com/user-attachments/assets/59ea0dba-5a4f-4c8d-9f4a-cfe503aacd4d


### Motor direito


https://github.com/user-attachments/assets/75047bc2-062f-4108-b39a-5d49f1da4bf1


## Testes dos Encoders

A seguir, são apresentados os testes realizados nos encoders acoplados aos motores do robô Micromouse. O procedimento consiste em realizar a rotação do eixo de cada motor por meio de um código desenvolvido e executado na Arduino IDE, permitindo monitorar e contabilizar os pulsos gerados pelos encoders durante o movimento.

O objetivo desse ensaio é determinar a quantidade de pulsos gerados a cada volta completa do eixo do motor, possibilitando caracterizar o comportamento dos encoders e verificar a consistência das leituras obtidas. As informações coletadas são fundamentais para o desenvolvimento do sistema de controle de movimento, uma vez que permitem estimar a rotação dos motores, acompanhar o deslocamento das rodas e fornecer dados para o controle de velocidade e posicionamento do robô.

Dessa forma, os testes permitem validar o funcionamento dos encoders e garantir que os sinais de realimentação sejam corretamente interpretados pelo microcontrolador, contribuindo para uma locomoção mais precisa e controlada durante a navegação do Micromouse.


### Encoder esquerdo


https://github.com/user-attachments/assets/de1ecd0c-4f3b-4ad0-9075-72a6d38e37d7


### Encoder direito


https://github.com/user-attachments/assets/9eab6e16-1f8a-4859-a0e0-7390c647fcf0


## Testes dos Sensores

A seguir, são apresentados os testes realizados nos sensores de proximidade do robô Micromouse. Para a realização do ensaio, foi desenvolvido um código na Arduino IDE responsável por realizar a leitura dos valores analógicos dos sensores e transmiti-los ao Monitor Serial, permitindo acompanhar as variações dos sinais em tempo real.

Durante o procedimento, as mãos foram aproximadas individualmente da região frontal de detecção dos sensores, simulando a presença de paredes e obstáculos no labirinto. Com essa aproximação, foram observadas as variações nos valores de conversão analógico-digital (ADC), exibidos diretamente no Monitor Serial da Arduino IDE.

O objetivo desse teste foi verificar a capacidade de detecção dos sensores, analisar suas respostas diante da aproximação de obstáculos e confirmar o funcionamento do processo de aquisição e transmissão dos dados ao microcontrolador. Essas informações são essenciais para o sistema de navegação do Micromouse, pois permitem identificar as paredes do labirinto e auxiliar na tomada de decisões durante sua movimentação autônoma.


https://github.com/user-attachments/assets/e10c2ab9-da3a-4600-b064-3b85f6d6397a


## Testes de Locomoção no Labirinto

Por fim, foram realizados os testes de integração do robô Micromouse em um ambiente que simula o labirinto, com o objetivo de avaliar o funcionamento conjunto dos sistemas de hardware durante a locomoção. Nessa etapa, o robô foi colocado diretamente no labirinto para verificar seu comportamento diante dos obstáculos e sua capacidade de realizar os movimentos necessários para percorrer os corredores.

Durante os ensaios, foram avaliados o acionamento e a sincronização dos motores, a resposta dos sensores de proximidade na identificação das paredes e a utilização dos encoders para o acompanhamento da rotação dos motores e do deslocamento das rodas. Dessa maneira, foi possível observar a interação entre os diferentes componentes e verificar se os sinais captados pelos sensores e encoders correspondiam adequadamente às condições de movimentação do robô.

Esse teste representa uma das etapas mais importantes da validação do hardware, pois permite avaliar o desempenho dos componentes não apenas individualmente, mas também em conjunto, considerando as condições reais de operação do projeto. Além disso, possibilita identificar eventuais dificuldades relacionadas à locomoção, ao alinhamento, à detecção de obstáculos e à execução de manobras, fornecendo informações importantes para a realização de ajustes e melhorias.

Assim, os testes no labirinto permitem verificar se a estrutura de hardware desenvolvida atende às necessidades de movimentação e navegação do Micromouse, contribuindo para a validação final do sistema e para o desenvolvimento das próximas etapas do projeto.


https://github.com/user-attachments/assets/cf46bdfd-20cd-4ac6-9191-b18ea2abefbe



# Testes de Integração

Confiram as regras de avaliação dos testes de integração no [documento do tema do projeto](https://docs.google.com/presentation/d/1hRx0g9mpVGQbTv44gLEoAyf4b4kKM4vnrABTnG8HlUk/edit?usp=sharing).

| **ID** | **Descrição** | **Data de Início** | **Data de Conclusão** | **Sub-equipes** | **Responsável** | **Resultados Esperados** | **Resultados Obtidos** | **Aprovação Interna (S/N)** | **Aprovação do Orientador (S/N)** | **Observações** |
|:------:|---------------|--------------------|-----------------------|----------------|-----------------|--------------------------|------------------------|:---------------------------:|:--------------------------------:|-----------------|
| 5.1 | | | | | | | | | | |
| 5.2 | | | | | | | | | | |
| 5.3 | | | | | | | | | | |
| 5.4 | | | | | | | | | | |
| 5.5 | | | | | | | | | | |
| 5.6 | | | | | | | | | | |
| 5.7 | | | | | | | | | | |
| 5.8 | | | | | | | | | | |
| 5.9 | | | | | | | | | | |
| 5.10 | | | | | | | | | | |
| 5.11 | | | | | | | | | | |
| 5.12 | | | | | | | | | | |
| 5.13 | | | | | | | | | | |
| 5.14 | | | | | | | | | | |
| 5.15 | | | | | | | | | | |
| 5.16 | | | | | | | | | | |
