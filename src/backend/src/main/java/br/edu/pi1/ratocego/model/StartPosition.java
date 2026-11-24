package br.edu.pi1.ratocego.model;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonValue;

import java.util.Locale;

public enum StartPosition {
    BOTTOM_LEFT("bottom_left"),
    BOTTOM_RIGHT("bottom_right");

    private final String jsonValue;

    StartPosition(String jsonValue) {
        this.jsonValue = jsonValue;
    }

    @JsonValue
    public String toJsonValue() {
        return jsonValue;
    }

    @JsonCreator
    public static StartPosition fromJson(String value) {
        if (value == null || value.isBlank()) {
            return null;
        }
        String normalized = value.trim().toLowerCase(Locale.ROOT);
        for (StartPosition candidate : values()) {
            if (candidate.jsonValue.equals(normalized)) {
                return candidate;
            }
        }
        throw new IllegalArgumentException("Invalid startPosition: " + value);
    }
}
