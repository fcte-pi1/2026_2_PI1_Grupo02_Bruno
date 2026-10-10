package br.edu.pi1.ratocego.service;

import br.edu.pi1.ratocego.dto.mqtt.RunFinishedPayload;
import br.edu.pi1.ratocego.dto.mqtt.RunInterruptedPayload;
import br.edu.pi1.ratocego.dto.mqtt.RunStartedPayload;
import br.edu.pi1.ratocego.dto.websocket.RunFinishedUpdate;
import br.edu.pi1.ratocego.dto.websocket.RunInterruptedUpdate;
import br.edu.pi1.ratocego.dto.websocket.RunStartedUpdate;
import br.edu.pi1.ratocego.model.Run;
import br.edu.pi1.ratocego.model.RunStatus;
import br.edu.pi1.ratocego.repository.RunRepository;
import br.edu.pi1.ratocego.util.MqttPayloadParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationEventPublisher;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.Instant;

@Service
public class RunLifecycleService {

    private static final Logger log = LoggerFactory.getLogger(RunLifecycleService.class);

    private final MqttPayloadParser payloadParser;
    private final RunRepository runRepository;
    private final ApplicationEventPublisher eventPublisher;

    public RunLifecycleService(MqttPayloadParser payloadParser, RunRepository runRepository,
                               ApplicationEventPublisher eventPublisher) {
        this.payloadParser = payloadParser;
        this.runRepository = runRepository;
        this.eventPublisher = eventPublisher;
    }

    @Transactional
    public void processStarted(String payload) {
        RunStartedPayload event = payloadParser.parseAndValidate(payload, RunStartedPayload.class);

        Run run = runRepository.findById(event.runId()).orElseGet(() -> newRun(event.runId()));
        if (run.getStatus() == RunStatus.IN_PROGRESS) {
            if (event.timestamp().equals(run.getStartedAt())) {
                log.warn("Ignoring duplicate run.started for runId={}", event.runId());
                return;
            }
            throw new IllegalStateException("Run is already in progress for runId=" + event.runId());
        }
        run.setStatus(run.getStatus().transitionTo(RunStatus.IN_PROGRESS));
        initializeRun(run, event);
        runRepository.save(run);
        eventPublisher.publishEvent(new WebSocketUpdateEvent(RunStartedUpdate.builder()
                .runId(event.runId())
                .timestamp(event.timestamp())
                .mazeRows(event.mazeRows())
                .mazeColumns(event.mazeColumns())
                .status(RunStatus.IN_PROGRESS)
                .build()));
    }

    @Transactional
    public void processFinished(String payload) {
        RunFinishedPayload event = payloadParser.parseAndValidate(payload, RunFinishedPayload.class);
        if (event.status() != RunStatus.COMPLETED && event.status() != RunStatus.FAILED) {
            throw new IllegalArgumentException("run.finished must include a terminal status");
        }

        Run run = findRun(event.runId());
        validateFinishTimestamp(run, event.timestamp(), "run.finished");
        run.setStatus(run.getStatus().transitionTo(event.status()));
        run.setFinishedAt(event.timestamp());
        run.setChallengeCompleted(event.challengeCompleted());
        runRepository.save(run);

        eventPublisher.publishEvent(new WebSocketUpdateEvent(RunFinishedUpdate.builder()
                .runId(event.runId())
                .timestamp(event.timestamp())
                .status(event.status())
                .challengeCompleted(event.challengeCompleted())
                .distanceTravelledMeters(run.getDistanceTravelledMeters())
                .averageSpeedMetersPerSecond(run.getAverageSpeedMetersPerSecond())
                .chargeConsumedMilliampHours(run.getChargeConsumedMilliampHours())
                .energyConsumedWattHours(run.getEnergyConsumedWattHours())
                .build()));
    }

    @Transactional
    public void processInterrupted(String payload) {
        RunInterruptedPayload event = payloadParser.parseAndValidate(payload, RunInterruptedPayload.class);

        Run run = findRun(event.runId());
        validateFinishTimestamp(run, event.timestamp(), "run.interrupted");
        run.setStatus(run.getStatus().transitionTo(RunStatus.INTERRUPTED));
        run.setFinishedAt(event.timestamp());
        run.setChallengeCompleted(null);
        runRepository.save(run);

        eventPublisher.publishEvent(new WebSocketUpdateEvent(RunInterruptedUpdate.builder()
                .runId(event.runId())
                .timestamp(event.timestamp())
                .status(RunStatus.INTERRUPTED)
                .build()));
    }

    private Run newRun(long runId) {
        Run run = new Run();
        run.setId(runId);
        run.setStatus(RunStatus.START_REQUESTED);
        return run;
    }

    private void initializeRun(Run run, RunStartedPayload event) {
        run.setStartedAt(event.timestamp());
        run.setFinishedAt(null);
        run.setMazeRows(event.mazeRows());
        run.setMazeColumns(event.mazeColumns());
        run.setChallengeCompleted(null);
        run.setDistanceTravelledMeters(BigDecimal.ZERO);
        run.setAverageSpeedMetersPerSecond(null);
        run.setChargeConsumedMilliampHours(null);
        run.setEnergyConsumedWattHours(null);
        run.setBatteryStatus(null);
        run.setLastSequence(0);
        run.setLastSampleAt(null);
        run.setLastCurrentMilliAmps(null);
        run.setLastPowerWatts(null);
    }

    private void validateFinishTimestamp(Run run, Instant timestamp, String eventName) {
        if (run.getStartedAt() == null || timestamp.isBefore(run.getStartedAt())
                || (run.getLastSampleAt() != null && timestamp.isBefore(run.getLastSampleAt()))) {
            throw new IllegalArgumentException(eventName + " timestamp precedes run start or latest telemetry");
        }
    }

    private Run findRun(long runId) {
        return runRepository.findById(runId)
                .orElseThrow(() -> new IllegalArgumentException("No run found for runId=" + runId));
    }

}
