package br.edu.pi1.ratocego.config;

import org.junit.jupiter.api.Test;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.mqtt.core.MqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.messaging.MessageHandler;
import org.springframework.test.util.ReflectionTestUtils;

import static org.junit.jupiter.api.Assertions.assertInstanceOf;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class MqttConfigTest {

    @Test
    void createsMqttFactoryChannelsInboundAdaptersAndOutboundHandler() {
        MqttConfig config = new MqttConfig();
        ReflectionTestUtils.setField(config, "clientId", "backend");
        ReflectionTestUtils.setField(config, "topicPrefix", "ratocego");
        ReflectionTestUtils.setField(config, "brokerUri", "tcp://localhost:1883");
        ReflectionTestUtils.setField(config, "qos", 1);
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
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttTelemetryInbound(factory, telemetryChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunStartedInbound(factory, startedChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunFinishedInbound(factory, finishedChannel));
        assertInstanceOf(MqttPahoMessageDrivenChannelAdapter.class, config.mqttRunInterruptedInbound(factory, interruptedChannel));
        MessageHandler handler = config.mqttOutbound(factory, "backend-out", 5000, 1);
        assertNotNull(handler);
    }
}
