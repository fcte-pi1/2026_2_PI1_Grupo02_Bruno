package br.edu.pi1.ratocego.adapter.mqtt;

import org.springframework.boot.health.contributor.Health;
import org.springframework.boot.health.contributor.HealthIndicator;
import org.springframework.context.event.EventListener;
import org.springframework.integration.mqtt.core.MqttPahoComponent;
import org.springframework.integration.mqtt.event.MqttConnectionFailedEvent;
import org.springframework.integration.mqtt.event.MqttIntegrationEvent;
import org.springframework.integration.mqtt.event.MqttSubscribedEvent;
import org.springframework.stereotype.Component;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Component("mqttConnection")
public class MqttConnectionHealthIndicator implements HealthIndicator {

    private static final Map<String, ConnectionState> INBOUND_ADAPTERS = Map.of(
            "mqttTelemetryInbound", ConnectionState.UNKNOWN,
            "mqttRunStartedInbound", ConnectionState.UNKNOWN,
            "mqttRunFinishedInbound", ConnectionState.UNKNOWN,
            "mqttRunInterruptedInbound", ConnectionState.UNKNOWN
    );

    private final Map<String, ConnectionState> states = new ConcurrentHashMap<>(INBOUND_ADAPTERS);

    @EventListener
    public void onSubscribed(MqttSubscribedEvent event) {
        if (isInboundAdapterEvent(event)) {
            states.put(inboundAdapterName(event), ConnectionState.CONNECTED);
        }
    }

    @EventListener
    public void onConnectionFailed(MqttConnectionFailedEvent event) {
        if (isInboundAdapterEvent(event)) {
            states.put(inboundAdapterName(event), ConnectionState.DISCONNECTED);
        }
    }

    @Override
    public Health health() {
        if (states.containsValue(ConnectionState.DISCONNECTED)) {
            return Health.down().withDetail("inboundAdapters", states).build();
        }
        if (states.containsValue(ConnectionState.UNKNOWN)) {
            return Health.unknown().withDetail("inboundAdapters", states).build();
        }
        return Health.up().withDetail("inboundAdapters", states).build();
    }

    private boolean isInboundAdapterEvent(MqttIntegrationEvent event) {
        MqttPahoComponent source = event.getSourceAsType();
        return states.containsKey(source.getBeanName());
    }

    private String inboundAdapterName(MqttIntegrationEvent event) {
        MqttPahoComponent source = event.getSourceAsType();
        return source.getBeanName();
    }

    private enum ConnectionState {
        UNKNOWN,
        CONNECTED,
        DISCONNECTED
    }
}
