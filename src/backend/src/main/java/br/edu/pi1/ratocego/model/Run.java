package br.edu.pi1.ratocego.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.Instant;

@Entity
@Table(name = "runs")
@Getter
@Setter
@NoArgsConstructor
public class Run {

    @Id
    private Long id;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    private RunStatus status;

    private Instant startedAt;
    private Instant finishedAt;
    private Integer mazeRows;
    private Integer mazeColumns;
    private Boolean challengeCompleted;
    private BigDecimal distanceTravelledMeters;
    private BigDecimal averageSpeedMetersPerSecond;
    private BigDecimal chargeConsumedMilliampHours;
    private BigDecimal energyConsumedWattHours;

    @Enumerated(EnumType.STRING)
    private BatteryStatus batteryStatus;

    @Column(nullable = false)
    private long lastSequence;

    private Instant lastSampleAt;
    private BigDecimal lastCurrentMilliAmps;
    private BigDecimal lastPowerWatts;
}
