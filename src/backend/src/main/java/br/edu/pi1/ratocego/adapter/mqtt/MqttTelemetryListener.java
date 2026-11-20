package br.edu.pi1.ratocego.adapter.mqtt;

import br.edu.pi1.ratocego.service.TelemetryService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.stereotype.Component;

@Component
public class MqttTelemetryListener {

    private static final Logger log = LoggerFactory.getLogger(MqttTelemetryListener.class);

    private final TelemetryService telemetryService;

    public MqttTelemetryListener(TelemetryService telemetryService) {
        this.telemetryService = telemetryService;
    }

    @ServiceActivator(inputChannel = "mqttTelemetryInputChannel")
    public void onTelemetrySample(@Payload String payload) {
        try {
            telemetryService.processSample(payload);
        } catch (RuntimeException exception) {
            log.warn("Ignoring rejected MQTT telemetry sample: {}", exception.getMessage());
        }
    }
}
