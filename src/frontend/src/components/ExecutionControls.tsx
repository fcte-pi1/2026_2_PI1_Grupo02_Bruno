import { Icon } from './Icon'
import {
  MAX_MAZE_DIMENSION,
  MIN_MAZE_DIMENSION,
  startPositionLabels,
  type StartPosition,
} from '../types/maze'
import type { RunResult } from '../types/telemetry'
import { getRunStatusAppearance } from '../utils/runStatus'

type Props = {
  mazeRows: number
  mazeColumns: number
  startPosition: StartPosition
  result: RunResult
  robotMoving: boolean
  onRowsChange: (rows: number) => void
  onColumnsChange: (columns: number) => void
  onStartPositionChange: (startPosition: StartPosition) => void
  onPrepare: () => void
  busy: boolean
  executionLocked: boolean
  canInterrupt: boolean
  onStart: () => void
  onInterrupt: () => void
  notice?: string
}

export function ExecutionControls({
  mazeRows,
  mazeColumns,
  startPosition,
  result,
  robotMoving,
  onRowsChange,
  onColumnsChange,
  onStartPositionChange,
  onPrepare,
  busy,
  executionLocked,
  canInterrupt,
  onStart,
  onInterrupt,
  notice,
}: Props) {
  const appearance = getRunStatusAppearance(result)
  const statusTone = appearance === 'active'
    ? 'border-[#695438] bg-[#302a20] text-amber'
    : appearance === 'success'
      ? 'border-[#4d6040] bg-[#252b22] text-green'
      : appearance === 'failure'
        ? 'border-[#65443d] bg-[#302321] text-danger'
        : 'border-[#424640] bg-[#292c29] text-secondary'
  const markTone = appearance === 'active'
    ? 'bg-amber'
    : appearance === 'success'
      ? 'bg-green'
      : appearance === 'failure' ? 'bg-danger' : 'bg-[#a0a39b]'
  const buttonBase = 'inline-flex min-h-10 cursor-pointer items-center justify-center gap-2 rounded-[3px] border px-3 py-1 text-small leading-[1.1] font-semibold transition-colors disabled:cursor-not-allowed disabled:opacity-[.42] max-[700px]:px-2 max-[700px]:text-caption'
  const inputBase = 'min-h-10 w-[68px] rounded-[3px] border border-[#4a4e48] bg-surface-low px-2.5 py-[7px] text-center font-mono text-small text-text tabular-nums disabled:cursor-not-allowed disabled:opacity-50'

  return (
    <section className="flex min-h-[66px] flex-wrap items-center gap-x-4 gap-y-3 rounded-[4px] border border-line border-l-[3px] border-l-[#7b8d7b] bg-surface p-[10px_14px] max-[960px]:gap-x-3 max-[960px]:gap-y-[11px] max-[700px]:p-[11px]" aria-label="Controles do percurso">
      <div className="flex flex-wrap items-center gap-2">
        <span className="font-mono text-caption text-secondary">Labirinto</span>
        <label className="sr-only" htmlFor="maze-cols-input">Colunas do labirinto (1 a 18)</label>
        <input
          className={inputBase}
          id="maze-cols-input"
          type="number"
          min={MIN_MAZE_DIMENSION}
          max={MAX_MAZE_DIMENSION}
          value={mazeColumns}
          disabled={executionLocked}
          onChange={(event) => onColumnsChange(Number(event.target.value))}
          title="Colunas (1 a 18)"
        />
        <span className="font-mono text-caption text-muted">×</span>
        <label className="sr-only" htmlFor="maze-rows-input">Linhas do labirinto (1 a 18)</label>
        <input
          className={inputBase}
          id="maze-rows-input"
          type="number"
          min={MIN_MAZE_DIMENSION}
          max={MAX_MAZE_DIMENSION}
          value={mazeRows}
          disabled={executionLocked}
          onChange={(event) => onRowsChange(Number(event.target.value))}
          title="Linhas (1 a 18)"
        />
      </div>

      <div className="flex items-center gap-2">
        <label className="font-mono text-caption text-secondary" htmlFor="start-pos-select">Início</label>
        <select
          className="min-h-10 cursor-pointer rounded-[3px] border border-[#4a4e48] bg-surface-low px-[10px] py-[7px] font-mono text-caption text-text disabled:cursor-not-allowed disabled:opacity-50"
          id="start-pos-select"
          value={startPosition}
          disabled={executionLocked}
          onChange={(event) => onStartPositionChange(event.target.value as StartPosition)}
        >
          {(Object.entries(startPositionLabels) as [StartPosition, string][]).map(([value, label]) => (
            <option key={value} value={value}>
              {label}
            </option>
          ))}
        </select>
      </div>

      <span className={`inline-flex items-center gap-2 whitespace-nowrap rounded-[2px] border px-[9px] py-1.5 font-mono text-caption ${robotMoving ? 'border-[#4d6040] bg-[#252b22] text-green' : 'border-[#424640] bg-[#292c29] text-secondary'}`}>
        <span className={`size-[7px] shrink-0 rounded-full ${robotMoving ? 'bg-green animate-pulse' : 'bg-[#a0a39b]'}`} />
        Robô: {robotMoving ? 'Em movimento' : 'Parado'}
      </span>

      <span className={`inline-flex items-center gap-2 whitespace-nowrap rounded-[2px] border px-[9px] py-1.5 font-mono text-caption ${statusTone}`}>
        <span className={`size-[7px] shrink-0 rounded-full ${markTone}`} />
        {result}
      </span>

      <div className="ml-auto flex items-center gap-2 max-[960px]:ml-0 max-[960px]:w-full max-[960px]:justify-end max-[700px]:grid max-[700px]:grid-cols-2">
        <button className={`${buttonBase} border-[#4a4e48] bg-[#292c29] text-[#d0d0c8] hover:border-[#777b70] hover:bg-[#333733]`} onClick={onPrepare} disabled={executionLocked}>
          <Icon name="layers" /> <span className="button-label">Preparar mapa</span>
        </button>
        <button
          className={`${buttonBase} border-transparent bg-[#a8bd8f] text-[#171b17] hover:bg-[#bacda4]`}
          disabled={busy || executionLocked}
          onClick={onStart}
        >
          <Icon name="play" /> <span className="button-label">Iniciar</span>
        </button>
        <button
          className={`${buttonBase} border-[#67453e] bg-[#302321] text-[#e4a195] hover:bg-[#422c27]`}
          disabled={busy || !canInterrupt}
          onClick={onInterrupt}
        >
          <Icon name="stop" /> <span className="button-label">Interromper</span>
        </button>
      </div>
      {notice && <p className="basis-full -mt-1 mb-0 text-small text-amber" role="status">{notice}</p>}
    </section>
  )
}
