import { Icon } from './Icon'
import { MetricCard } from './MetricCard'
import { headingLabels, startPositionLabels, type MazeDimensions, type StartPosition } from '../types/maze'
import type { RunResult, Telemetry } from '../types/telemetry'
import { formatNumber } from '../utils/format'
import { getRunResultIcon, getRunStatusAppearance } from '../utils/runStatus'

type Props = {
  telemetry: Telemetry
  clock: string
  elapsedSeconds: number
  result: RunResult
  dimensions: MazeDimensions
  startPosition: StartPosition
  robotMoving: boolean
  goalReached: boolean
}

export function ExecutionMetrics({
  telemetry,
  clock,
  elapsedSeconds,
  result,
  dimensions,
  startPosition,
  robotMoving,
  goalReached,
}: Props) {
  const isBatteryLow = telemetry.batteryStatus === 'LOW'
  const effectiveResult: RunResult = goalReached ? 'Sucesso' : result
  const appearance = getRunStatusAppearance(effectiveResult)
  const resultTone = appearance === 'success'
    ? 'border-l-green text-green'
    : appearance === 'active'
      ? 'border-l-amber text-amber'
      : appearance === 'failure'
        ? 'border-l-danger text-danger'
        : 'border-l-[#6a7067] text-text'
  const ResultIcon = getRunResultIcon(effectiveResult)
  const isResultActive = !goalReached && (result === 'Em andamento' || result === 'Início solicitado' || result === 'Interrupção solicitada')
  const resultLabel = goalReached || result === 'Sucesso' ? 'Labirinto concluído' : result

  const batteryVoltage = telemetry.batteryVoltageVolts
  const currentMilliAmps = telemetry.currentMilliAmps
  const powerWatts = telemetry.currentPowerWatts
    ?? (batteryVoltage != null && currentMilliAmps != null ? (batteryVoltage * currentMilliAmps) / 1000 : undefined)
  const derivedAverageSpeed =
    telemetry.averageSpeedMetersPerSecond != null && telemetry.averageSpeedMetersPerSecond > 0
      ? telemetry.averageSpeedMetersPerSecond
      : telemetry.distanceTravelledMeters != null && elapsedSeconds > 0
        ? telemetry.distanceTravelledMeters / elapsedSeconds
        : telemetry.currentSpeedMetersPerSecond ?? telemetry.averageSpeedMetersPerSecond
  const position = telemetry.position
  const headingText = position?.heading
    ? `${headingLabels[position.heading] ?? position.heading} (${position.heading})`
    : '—'

  return (
    <aside className="flex flex-col gap-2.5" aria-label="Dados da execução">
      <div className="rounded-[4px] border border-line bg-surface p-[13px]">
        <p className="mb-2 mt-0 font-mono text-[11px] tracking-[.05em] text-muted">ESTADO DO ROBÔ</p>
        <div className="flex items-baseline justify-between gap-3">
          <span className="text-caption text-secondary">Movimento</span>
          <span className={`font-mono text-small font-medium ${robotMoving ? 'text-green' : 'text-secondary'}`}>
            {robotMoving ? 'Em movimento' : 'Parado'}
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Posição atual</span>
          <span className="font-mono text-small font-medium tabular-nums text-text">
            {position ? `(${position.row}, ${position.column})` : '—'}
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Orientação</span>
          <span className="font-mono text-small font-medium text-text">
            {headingText}
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Labirinto</span>
          <span className="font-mono text-small text-secondary">{dimensions.label}</span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Partida</span>
          <span className="text-right font-mono text-[11px] text-secondary">
            {startPositionLabels[startPosition]}
          </span>
        </div>
      </div>

      <div className="rounded-[4px] border border-line bg-surface p-[13px]">
        <p className="mb-2 mt-0 font-mono text-[11px] tracking-[.05em] text-muted">MONITORAMENTO ELÉTRICO (INA219)</p>
        <div className="flex items-baseline justify-between gap-3">
          <span className="text-caption text-secondary">Tensão da bateria</span>
          <span className={`whitespace-nowrap font-mono text-sm leading-[1.2] font-medium tabular-nums ${isBatteryLow ? 'text-danger' : 'text-green'}`}>
            {batteryVoltage == null ? '—' : formatNumber(batteryVoltage, 2)} <small className="text-caption font-normal text-secondary">V</small>
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Corrente elétrica</span>
          <span className="whitespace-nowrap font-mono text-sm leading-[1.2] font-medium tabular-nums text-cyan">
            {currentMilliAmps == null ? '—' : formatNumber(currentMilliAmps, 0)} <small className="text-caption font-normal text-secondary">mA</small>
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Potência consumida</span>
          <span className="whitespace-nowrap font-mono text-sm leading-[1.2] font-medium tabular-nums text-amber">
            {powerWatts == null ? '—' : formatNumber(powerWatts, 2)} <small className="text-caption font-normal text-secondary">W</small>
          </span>
        </div>
        <div className="mt-[8px] flex items-baseline justify-between gap-3 border-t border-line pt-[8px]">
          <span className="text-caption text-secondary">Carga acumulada</span>
          <span className={`whitespace-nowrap font-mono text-small leading-[1.2] font-medium tabular-nums ${isBatteryLow ? 'text-danger' : 'text-secondary'}`}>
            {formatNumber(telemetry.chargeConsumedMilliampHours, 1)} <small className="text-caption font-normal text-secondary">mAh</small>
          </span>
        </div>
        {isBatteryLow && (
          <p className="mt-2.5 mb-0 flex items-center gap-1.5 text-small text-danger">
            <Icon name="alert" /> Bateria em nível crítico
          </p>
        )}
      </div>

      <MetricCard
        label="Velocidade média"
        value={formatNumber(derivedAverageSpeed)}
        unit="m/s"
        icon="speed"
      />
      <MetricCard label="Tempo de execução" value={clock} icon="time" mono />

      <div className={`rounded-[4px] border border-line border-l-[3px] bg-surface p-[13px] ${resultTone}`}>
        <span className="text-caption text-secondary">Conclusão do labirinto</span>
        <div className="mt-2 flex items-center gap-2 text-body font-semibold">
          <span className={`grid size-[21px] place-items-center rounded-full ${appearance === 'success' ? 'bg-green text-[#1b2118]' : appearance === 'active' ? 'bg-amber text-[#211c14]' : appearance === 'failure' ? 'bg-danger text-[#241715]' : 'bg-[#353026] text-amber'}`}><ResultIcon className={`size-[13px] stroke-[2.5] ${isResultActive ? 'animate-spin' : ''}`} aria-hidden="true" /></span>
          {resultLabel}
        </div>
      </div>
    </aside>
  )
}
