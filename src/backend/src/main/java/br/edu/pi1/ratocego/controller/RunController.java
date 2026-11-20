package br.edu.pi1.ratocego.controller;

import br.edu.pi1.ratocego.dto.rest.RunResponse;
import br.edu.pi1.ratocego.dto.rest.TelemetrySampleResponse;
import br.edu.pi1.ratocego.model.MazeType;
import br.edu.pi1.ratocego.model.StartPosition;
import br.edu.pi1.ratocego.service.RunQueryService;
import br.edu.pi1.ratocego.service.RunLifecycleService;
import br.edu.pi1.ratocego.service.RunCommandPublicationException;
import br.edu.pi1.ratocego.mapper.RunMapper;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import org.springframework.http.ResponseEntity;
import org.springframework.http.HttpStatus;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/api/runs")
public class RunController {

    private final RunQueryService runQueryService;
    private final RunLifecycleService runLifecycleService;
    private final RunMapper runMapper;

    public RunController(RunQueryService runQueryService, RunLifecycleService runLifecycleService, RunMapper runMapper) {
        this.runQueryService = runQueryService;
        this.runLifecycleService = runLifecycleService;
        this.runMapper = runMapper;
    }

    public record StartRunRequest(
            MazeType mazeType,
            @Min(1) @Max(18) Integer mazeRows,
            @Min(1) @Max(18) Integer mazeColumns,
            StartPosition startPosition
    ) { }

    @PostMapping
    public ResponseEntity<RunResponse> startRun(@Valid @RequestBody StartRunRequest request) {
        try {
            if (request.mazeRows() != null || request.mazeColumns() != null) {
                if (request.mazeRows() == null || request.mazeColumns() == null) {
                    throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Informe mazeRows e mazeColumns");
                }
                return ResponseEntity.accepted().body(runMapper.toRunResponse(
                        runLifecycleService.start(request.mazeRows(), request.mazeColumns(), request.startPosition())));
            }
            if (request.mazeType() == null) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Informe mazeRows e mazeColumns (1 a 18) ou mazeType");
            }
            if (request.startPosition() == null) {
                return ResponseEntity.accepted().body(runMapper.toRunResponse(runLifecycleService.start(request.mazeType())));
            }
            int columns = switch (request.mazeType()) {
                case GRID_4X4 -> 4;
                case GRID_8X4 -> 8;
                case GRID_12X4 -> 12;
            };
            return ResponseEntity.accepted().body(runMapper.toRunResponse(
                    runLifecycleService.start(4, columns, request.startPosition())));
        } catch (RunCommandPublicationException exception) {
            throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE, exception.getMessage(), exception);
        } catch (IllegalArgumentException exception) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, exception.getMessage(), exception);
        } catch (IllegalStateException exception) {
            throw new ResponseStatusException(HttpStatus.CONFLICT, exception.getMessage(), exception);
        }
    }

    @PostMapping("/{runId}/interrupt")
    public ResponseEntity<RunResponse> interruptRun(@PathVariable long runId) {
        try {
            return ResponseEntity.accepted().body(runMapper.toRunResponse(runLifecycleService.interrupt(runId)));
        } catch (RunCommandPublicationException exception) {
            throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE, exception.getMessage(), exception);
        } catch (IllegalArgumentException exception) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, exception.getMessage(), exception);
        } catch (IllegalStateException exception) {
            throw new ResponseStatusException(HttpStatus.CONFLICT, exception.getMessage(), exception);
        }
    }

    @GetMapping("/active")
    public ResponseEntity<RunResponse> getActiveRun() {
        return ResponseEntity.of(runQueryService.findActiveRun());
    }

    // Sem filtros: consulta geral (HU51). Com mazeRows/mazeColumns ou mazeType: consulta por labirinto (HU50)
    @GetMapping
    public List<RunResponse> getRuns(
            @RequestParam(required = false) MazeType mazeType,
            @RequestParam(required = false) Integer mazeRows,
            @RequestParam(required = false) Integer mazeColumns) {
        if (mazeRows != null || mazeColumns != null) {
            if (mazeRows == null || mazeColumns == null
                    || mazeRows < RunLifecycleService.MIN_MAZE_DIMENSION || mazeRows > RunLifecycleService.MAX_MAZE_DIMENSION
                    || mazeColumns < RunLifecycleService.MIN_MAZE_DIMENSION || mazeColumns > RunLifecycleService.MAX_MAZE_DIMENSION) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "mazeRows e mazeColumns devem estar entre 1 e 18");
            }
            return runQueryService.findRunsByDimensions(mazeRows, mazeColumns);
        }
        return mazeType == null ? runQueryService.findAllRuns() : runQueryService.findRunsByMaze(mazeType);
    }

    @GetMapping("/{runId}")
    public ResponseEntity<RunResponse> getRunById(@PathVariable long runId) {
        return ResponseEntity.of(runQueryService.findRunById(runId));
    }

    @GetMapping("/{runId}/telemetry")
    public ResponseEntity<List<TelemetrySampleResponse>> getRunTelemetry(@PathVariable long runId) {
        return ResponseEntity.of(runQueryService.findRunTelemetry(runId));
    }
}
