package br.edu.pi1.ratocego.repository;

import br.edu.pi1.ratocego.model.TelemetrySample;
import org.springframework.data.jpa.repository.JpaRepository;

public interface TelemetrySampleRepository extends JpaRepository<TelemetrySample, Long> {

    boolean existsByEventId(long eventId);

    boolean existsByRun_IdAndSequence(long runId, long sequence);
}
