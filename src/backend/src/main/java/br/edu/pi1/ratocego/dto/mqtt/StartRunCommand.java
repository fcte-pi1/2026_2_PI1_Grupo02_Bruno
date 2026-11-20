package br.edu.pi1.ratocego.dto.mqtt;

import br.edu.pi1.ratocego.model.MazeType;
import br.edu.pi1.ratocego.model.StartPosition;

import java.time.Instant;

public record StartRunCommand(
        long eventId,
        long runId,
        MazeType mazeType,
        int mazeRows,
        int mazeColumns,
        StartPosition startPosition,
        Instant timestamp) {
}
