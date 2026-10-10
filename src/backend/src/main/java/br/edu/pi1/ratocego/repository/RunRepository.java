package br.edu.pi1.ratocego.repository;

import br.edu.pi1.ratocego.model.Run;
import org.springframework.data.jpa.repository.JpaRepository;

public interface RunRepository extends JpaRepository<Run, Long> {
}
