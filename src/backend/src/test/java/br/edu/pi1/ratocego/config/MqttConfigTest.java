package br.edu.pi1.ratocego.config;

import org.junit.jupiter.api.Test;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.mqtt.core.MqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.messaging.MessageHandler;

import static org.junit.jupiter.api.Assertions.assertInstanceOf;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class MqttConfigTest {

    @Test
    void createsMqttFactoryChannelsInboundAdaptersAndOutboundHandler() {
        MqttConfig config = new MqttConfig();
        MqttPahoClientFactory factory = config.mqttClientFactory("user", "secret", 7, 13, true);
        var telemetryChannel = config.mqttTelemetryInputChannel();
        var startedChannel = config.mqttRunStartedInputChannel();
        var finishedChannel = config.mqttRunFinishedInputChannel();
        var interruptedChannel = config.mqttRunInterruptedInputChannel();
        var outboundChannel = config.mqttOutboundChannel();

        assertNotNull(factory.getConnectionOptions());
        assertInstanceOf(DirectChannel.class, telemetryChannel);
        assertInstanceOf(DirectChannel.class, startedChannel);
        assertInstanceOf(DirectChannel.class, finishedChannel);
        assertInstanceOf(DirectChannel.class, interruptedChannel);
        assertInstanceOf(DirectChannel.class, outboundChannel);
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttTelemetryInbound(
                "backend", "ratocego", "tcp://localhost:1883", 1, factory, telemetryChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunStartedInbound(
                "backend", "ratocego", "tcp://localhost:1883", 1, factory, startedChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunFinishedInbound(
                "backend", "ratocego", "tcp://localhost:1883", 1, factory, finishedChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunInterruptedInbound(
                "backend", "ratocego", "tcp://localhost:1883", 1, factory, interruptedChannel));
        MessageHandler handler = config.mqttOutbound(factory, "backend-out", 5000, 1);
        assertNotNull(handler);
    }
}
