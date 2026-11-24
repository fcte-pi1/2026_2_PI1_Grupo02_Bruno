package br.edu.pi1.ratocego.dto.mqtt;

import br.edu.pi1.ratocego.model.StartPosition;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Positive;

import java.time.Instant;

public record RunStartedPayload(
        @Positive long eventId,
        @Positive long runId,
        @NotNull Instant timestamp,
        @Min(1) @Max(18) int mazeRows,
        @Min(1) @Max(18) int mazeColumns,
        StartPosition startPosition) {

    public RunStartedPayload(long eventId, long runId, Instant timestamp, int mazeRows, int mazeColumns) {
        this(eventId, runId, timestamp, mazeRows, mazeColumns, null);
    }
}
