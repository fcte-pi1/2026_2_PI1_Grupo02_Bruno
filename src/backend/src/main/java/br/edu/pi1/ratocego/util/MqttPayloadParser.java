package br.edu.pi1.ratocego.util;

import jakarta.validation.ConstraintViolation;
import jakarta.validation.ConstraintViolationException;
import jakarta.validation.Validator;
import org.springframework.stereotype.Component;
import tools.jackson.databind.ObjectMapper;

import java.util.HashSet;

@Component
public class MqttPayloadParser {

    private final ObjectMapper objectMapper;
    private final Validator validator;

    public MqttPayloadParser(ObjectMapper objectMapper, Validator validator) {
        this.objectMapper = objectMapper;
        this.validator = validator;
    }

    public <T> T parseAndValidate(String payload, Class<T> payloadType) {
        T parsed;
        try {
            parsed = objectMapper.readValue(payload, payloadType);
        } catch (Exception exception) {
            throw new IllegalArgumentException(
                    "Invalid MQTT JSON payload for " + payloadType.getSimpleName(), exception);
        }

        var violations = validator.validate(parsed);
        if (!violations.isEmpty()) {
            throw new ConstraintViolationException(new HashSet<ConstraintViolation<?>>(violations));
        }
        return parsed;
    }
}
