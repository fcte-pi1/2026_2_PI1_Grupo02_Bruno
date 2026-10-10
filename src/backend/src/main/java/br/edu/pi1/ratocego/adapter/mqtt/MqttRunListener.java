package br.edu.pi1.ratocego.adapter.mqtt;

import br.edu.pi1.ratocego.service.RunLifecycleService;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.stereotype.Component;

@Component
public class MqttRunListener {

    private final RunLifecycleService runLifecycleService;

    public MqttRunListener(RunLifecycleService runLifecycleService) {
        this.runLifecycleService = runLifecycleService;
    }

    @ServiceActivator(inputChannel = "mqttRunStartedInputChannel")
    public void onRunStarted(
            @Payload String payload) {
        runLifecycleService.processStarted(payload);
    }

    @ServiceActivator(inputChannel = "mqttRunFinishedInputChannel")
    public void onRunFinished(@Payload String payload) {
        runLifecycleService.processFinished(payload);
    }

    @ServiceActivator(inputChannel = "mqttRunInterruptedInputChannel")
    public void onRunInterrupted(@Payload String payload) {
        runLifecycleService.processInterrupted(payload);
    }
}
