package br.edu.pi1.ratocego.util;

import br.edu.pi1.ratocego.dto.mqtt.TelemetrySamplePayload;
import br.edu.pi1.ratocego.model.Heading;
import br.edu.pi1.ratocego.model.Position;
import jakarta.validation.Validator;
import org.junit.jupiter.api.Test;
import tools.jackson.databind.ObjectMapper;

import java.util.Set;
import java.math.BigDecimal;
import java.time.Instant;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class MqttPayloadParserTest {

    @Test
    void parsesAndReturnsValidatedPayload() {
        ObjectMapper mapper = mock(ObjectMapper.class);
        Validator validator = mock(Validator.class);
        TelemetrySamplePayload payload = new TelemetrySamplePayload(1, 2, 3, Instant.now(),
                new Position(0, 0, Heading.NORTH), BigDecimal.ZERO, BigDecimal.ZERO,
                BigDecimal.ONE, BigDecimal.ZERO);
        when(mapper.readValue("{}", TelemetrySamplePayload.class)).thenReturn(payload);
        when(validator.validate(payload)).thenReturn(Set.of());

        TelemetrySamplePayload result = new MqttPayloadParser(mapper, validator)
                .parseAndValidate("{}", TelemetrySamplePayload.class);

        assertEquals(payload, result);
    }

    @Test
    void wrapsMalformedJsonAsInvalidPayload() {
        ObjectMapper mapper = mock(ObjectMapper.class);
        Validator validator = mock(Validator.class);
        when(mapper.readValue("broken", TelemetrySamplePayload.class))
                .thenThrow(new IllegalArgumentException("invalid json"));

        assertThrows(IllegalArgumentException.class,
                () -> new MqttPayloadParser(mapper, validator).parseAndValidate("broken", TelemetrySamplePayload.class));
    }
}
