import { useCallback, useEffect, useRef, useState } from 'react'
import { useTelemetryRealtime, type RealtimeMessage } from './realtime/useTelemetryRealtime'
import { AppHeader } from './components/AppHeader'
import { ConnectionStatus } from './components/ConnectionStatus'
import { ExecutionControls } from './components/ExecutionControls'
import { ExecutionMetrics } from './components/ExecutionMetrics'
import { MazePanel } from './components/MazePanel'
import { clampMazeDimension, getGoalCell } from './types/maze'
import type { MazeDimensions, Position, StartPosition } from './types/maze'
import type { RunResult, Telemetry } from './types/telemetry'
import { formatElapsedTime } from './utils/format'
import { fetchActiveRun, fetchRun, fetchRunTelemetry, interruptRun, startRun } from './api/runs'

function App() {
  const [mazeRows, setMazeRows] = useState(4)
  const [mazeColumns, setMazeColumns] = useState(4)
  const [startPosition, setStartPosition] = useState<StartPosition>('bottom_left')
  const [dimensions, setDimensions] = useState<MazeDimensions>({ rows: 4, columns: 4, label: '4 × 4' })
  const [route, setRoute] = useState<Position[]>([])
  const [telemetry, setTelemetry] = useState<Telemetry>({})
  const [startedAt, setStartedAt] = useState<number | null>(null)
  const [elapsed, setElapsed] = useState(0)
  const [result, setResult] = useState<RunResult>('Aguardando')
  const [runId, setRunId] = useState<number | null>(null)
  const [action, setAction] = useState<'starting' | 'interrupting' | null>(null)
  const [notice, setNotice] = useState('')
  const [interruptRetryAllowed, setInterruptRetryAllowed] = useState(false)

  const runIdRef = useRef<number | null>(runId)
  runIdRef.current = runId
  const dimensionsRef = useRef(dimensions)
  dimensionsRef.current = dimensions

  const handleRealtimeMessage = useCallback((message: RealtimeMessage) => {
    const activeRunId = runIdRef.current

    if (message.kind === 'started') {
      const eventRunId = Number(message.payload.runId)
      if (activeRunId !== null && eventRunId !== activeRunId) return
      if (Number.isFinite(eventRunId)) {
        runIdRef.current = eventRunId
        setRunId(eventRunId)
      }
      setAction(null)
      setNotice('')
      setInterruptRetryAllowed(false)
      const rows = Number(message.payload.mazeRows)
      const columns = Number(message.payload.mazeColumns)
      const eventStartPos = message.payload.startPosition

      if (rows > 0 && columns > 0) {
        const nextDimensions = { rows, columns, label: `${columns} × ${rows}` }
        dimensionsRef.current = nextDimensions
        setMazeRows(rows)
        setMazeColumns(columns)
        setDimensions(nextDimensions)
      }
      if (eventStartPos === 'bottom_left' || eventStartPos === 'bottom_right') {
        setStartPosition(eventStartPos)
      }

      setRoute([])
      setTelemetry({})
      setResult('Em andamento')
      setStartedAt(Date.now())
      setElapsed(0)
      return
    }

    if (message.kind === 'telemetry') {
      const sample = message.payload as Telemetry
      const eventRunId = Number(message.payload.runId)
      if (!Number.isFinite(eventRunId)) return
      if (activeRunId !== null && eventRunId !== activeRunId) return
      if (activeRunId === null) {
        runIdRef.current = eventRunId
        setRunId(eventRunId)
        setResult('Em andamento')
        setAction(null)
        setNotice('')
      }
      const position = sample.position

      setTelemetry((previous) => ({
        ...previous,
        ...sample,
        averageSpeedMetersPerSecond: sample.averageSpeedMetersPerSecond ?? previous.averageSpeedMetersPerSecond,
        chargeConsumedMilliampHours: sample.chargeConsumedMilliampHours ?? previous.chargeConsumedMilliampHours,
        energyConsumedWattHours: sample.energyConsumedWattHours ?? previous.energyConsumedWattHours,
      }))

      if (isPositionInsideMaze(position, dimensionsRef.current)) {
        setRoute((currentRoute) => {
          const previousPosition = currentRoute[currentRoute.length - 1]
          const positionUnchanged =
            previousPosition?.row === position.row
            && previousPosition?.column === position.column
            && previousPosition?.heading === position.heading

          if (positionUnchanged) return currentRoute
          if (previousPosition?.row === position.row && previousPosition?.column === position.column) {
            return [...currentRoute.slice(0, -1), position]
          }
          return [...currentRoute, position]
        })
      }

      return
    }

    if (message.kind === 'finished') {
      if (activeRunId !== null && Number(message.payload.runId) !== activeRunId) return
      setAction(null)
      setTelemetry((previous) => ({
        ...previous,
        distanceTravelledMeters: typeof message.payload.distanceTravelledMeters === 'number'
          ? message.payload.distanceTravelledMeters
          : previous.distanceTravelledMeters,
        averageSpeedMetersPerSecond: typeof message.payload.averageSpeedMetersPerSecond === 'number'
          ? message.payload.averageSpeedMetersPerSecond
          : previous.averageSpeedMetersPerSecond,
        chargeConsumedMilliampHours: typeof message.payload.chargeConsumedMilliampHours === 'number'
          ? message.payload.chargeConsumedMilliampHours
          : previous.chargeConsumedMilliampHours,
        energyConsumedWattHours: typeof message.payload.energyConsumedWattHours === 'number'
          ? message.payload.energyConsumedWattHours
          : previous.energyConsumedWattHours,
      }))
      setResult(
        message.payload.challengeCompleted === true
          ? 'Sucesso'
          : message.payload.challengeCompleted === false
            ? 'Não concluído'
            : 'Resultado indisponível',
      )
      return
    }

    if (message.kind === 'interrupted') {
      if (activeRunId !== null && Number(message.payload.runId) !== activeRunId) return
      setAction(null)
      setResult('Interrompido')
      setNotice('')
      setInterruptRetryAllowed(false)
      return
    }

    if (message.kind === 'status') {
      const eventRunId = Number(message.payload.runId)
      if (activeRunId !== null && eventRunId !== activeRunId) return
      const state = String(message.payload.status ?? '').toUpperCase()
      if (state === 'START_REQUESTED' || state === 'PENDING') {
        if (Number.isFinite(eventRunId)) {
          runIdRef.current = eventRunId
          setRunId(eventRunId)
        }
        setResult('Início solicitado')
        setNotice(String(message.payload.message ?? 'Aguardando confirmação do robô…'))
        setInterruptRetryAllowed(false)
      } else if (state === 'INTERRUPT_REQUESTED') {
        if (Number.isFinite(eventRunId)) {
          runIdRef.current = eventRunId
          setRunId(eventRunId)
        }
        setResult('Interrupção solicitada')
        setInterruptRetryAllowed(message.payload.retryAllowed === true)
        setNotice(String(message.payload.message ?? 'Aguardando confirmação da interrupção…'))
      } else if (state === 'FAILED') {
        setResult('Falhou')
        setAction(null)
        setNotice(String(message.payload.message ?? 'A execução falhou.'))
        setInterruptRetryAllowed(false)
      }
      if (state === 'INTERRUPT_REQUESTED' && message.payload.retryAllowed === true) setAction(null)
    }
  }, [])

  const { connected, lastMessageAt } = useTelemetryRealtime(handleRealtimeMessage)

  useEffect(() => {
    let cancelled = false

    async function restoreActiveRun() {
      try {
        const activeRun = await fetchActiveRun()
        if (cancelled) return
        if (!activeRun) {
          if (runId !== null) {
            const knownRun = await fetchRun(runId)
            if (cancelled || !knownRun) return
            setAction(null)
            if (knownRun.status === 'COMPLETED') {
              setResult(knownRun.challengeCompleted === true ? 'Sucesso' : knownRun.challengeCompleted === false ? 'Não concluído' : 'Resultado indisponível')
            } else if (knownRun.status === 'INTERRUPTED') {
              setResult('Interrompido')
            } else if (knownRun.status === 'FAILED') {
              setResult('Falhou')
            }
          }
          return
        }

        setAction(null)
        setRunId(activeRun.id)
        if (activeRun.mazeRows && activeRun.mazeColumns) {
          setMazeRows(activeRun.mazeRows)
          setMazeColumns(activeRun.mazeColumns)
          setDimensions({
            rows: activeRun.mazeRows,
            columns: activeRun.mazeColumns,
            label: `${activeRun.mazeColumns} × ${activeRun.mazeRows}`,
          })
        }
        if (activeRun.startPosition === 'bottom_left' || activeRun.startPosition === 'bottom_right') {
          setStartPosition(activeRun.startPosition)
        }

        if (activeRun.status === 'START_REQUESTED') {
          setResult('Início solicitado')
          setNotice('Aguardando confirmação do robô…')
          setStartedAt(null)
        } else {
          const startTime = activeRun.startedAt ? Date.parse(activeRun.startedAt) : Number.NaN
          setStartedAt(Number.isFinite(startTime) ? startTime : null)
          setElapsed(Number.isFinite(startTime) ? Math.max(0, Math.floor((Date.now() - startTime) / 1_000)) : 0)
          if (activeRun.status === 'INTERRUPT_REQUESTED') {
            const requestTime = activeRun.interruptRequestedAt ? Date.parse(activeRun.interruptRequestedAt) : Number.NaN
            const retryAllowed = Number.isFinite(requestTime) && Date.now() - requestTime >= 2_000
            setResult('Interrupção solicitada')
            setInterruptRetryAllowed(retryAllowed)
            setNotice(retryAllowed
              ? 'A interrupção não foi confirmada. Você pode tentar novamente.'
              : 'Aguardando confirmação da interrupção…')
          } else {
            setResult('Em andamento')
          }
        }

        const samples = await fetchRunTelemetry(activeRun.id)
        if (cancelled || samples.length === 0) return
        setRoute(samples.map((sample) => ({ row: sample.row, column: sample.column, heading: sample.heading })))
        const latest = samples[samples.length - 1]
        setTelemetry((previous) => ({
          ...previous,
          position: { row: latest.row, column: latest.column, heading: latest.heading },
          distanceTravelledMeters: latest.distanceTravelledMeters ?? activeRun.distanceTravelledMeters ?? previous.distanceTravelledMeters,
          currentSpeedMetersPerSecond: latest.currentSpeedMetersPerSecond ?? previous.currentSpeedMetersPerSecond,
          averageSpeedMetersPerSecond: activeRun.averageSpeedMetersPerSecond ?? previous.averageSpeedMetersPerSecond ?? latest.currentSpeedMetersPerSecond ?? undefined,
          batteryVoltageVolts: latest.batteryVoltageVolts ?? previous.batteryVoltageVolts,
          currentMilliAmps: latest.currentMilliAmps ?? previous.currentMilliAmps,
          currentPowerWatts: latest.currentPowerWatts ?? previous.currentPowerWatts,
          chargeConsumedMilliampHours: activeRun.chargeConsumedMilliampHours ?? previous.chargeConsumedMilliampHours,
          energyConsumedWattHours: activeRun.energyConsumedWattHours ?? previous.energyConsumedWattHours,
        }))
      } catch (error) {
        if (!cancelled) setNotice(error instanceof Error ? error.message : 'Não foi possível restaurar a execução ativa.')
      }
    }

    void restoreActiveRun()
    return () => { cancelled = true }
  }, [connected, runId])

  const stale =
    result === 'Em andamento' && Date.now() - (lastMessageAt ?? startedAt ?? Date.now()) > 1_000
  const status = !connected
    ? 'Desconectado'
    : stale
      ? 'Telemetria desatualizada'
      : lastMessageAt
        ? 'Ao vivo'
        : 'Conectado'
  const clock = formatElapsedTime(elapsed, startedAt !== null)

  useEffect(() => {
    const timer = window.setInterval(() => {
      if (startedAt !== null && result === 'Em andamento') {
        setElapsed(Math.floor((Date.now() - startedAt) / 1_000))
      }
    }, 250)

    return () => window.clearInterval(timer)
  }, [startedAt, result])

  // WebSocket updates are immediate; polling the persisted run state recovers a
  // terminal event if the browser disconnects or the broker drops its update.
  useEffect(() => {
    if (runId === null || !isExecutionLocked(result)) return
    const activeRunId = runId
    let cancelled = false
    let checking = false

    async function refreshRunStatus() {
      if (checking) return
      checking = true
      try {
        const run = await fetchRun(activeRunId)
        if (cancelled || !run) return
        setTelemetry((previous) => ({
          ...previous,
          distanceTravelledMeters: run.distanceTravelledMeters ?? previous.distanceTravelledMeters,
          averageSpeedMetersPerSecond: run.averageSpeedMetersPerSecond ?? previous.averageSpeedMetersPerSecond,
          chargeConsumedMilliampHours: run.chargeConsumedMilliampHours ?? previous.chargeConsumedMilliampHours,
          energyConsumedWattHours: run.energyConsumedWattHours ?? previous.energyConsumedWattHours,
        }))
        if (run.status === 'COMPLETED') {
          setResult(run.challengeCompleted === true
            ? 'Sucesso'
            : run.challengeCompleted === false ? 'Não concluído' : 'Resultado indisponível')
          setAction(null)
          setNotice('')
          setInterruptRetryAllowed(false)
        } else if (run.status === 'INTERRUPTED') {
          setResult('Interrompido')
          setAction(null)
          setNotice('')
          setInterruptRetryAllowed(false)
        } else if (run.status === 'FAILED') {
          setResult('Falhou')
          setAction(null)
          setNotice('A execução falhou.')
          setInterruptRetryAllowed(false)
        } else if (run.status === 'INTERRUPT_REQUESTED') {
          const requestTime = run.interruptRequestedAt ? Date.parse(run.interruptRequestedAt) : Number.NaN
          const retryAllowed = Number.isFinite(requestTime) && Date.now() - requestTime >= 2_000
          setResult('Interrupção solicitada')
          setInterruptRetryAllowed((previous) => previous || retryAllowed)
        } else if (run.status === 'IN_PROGRESS') {
          const startTime = run.startedAt ? Date.parse(run.startedAt) : Number.NaN
          if (Number.isFinite(startTime)) {
            setStartedAt(startTime)
            setElapsed(Math.max(0, Math.floor((Date.now() - startTime) / 1_000)))
          }
          setResult('Em andamento')
          setAction(null)
        } else if (run.status === 'START_REQUESTED') {
          setResult('Início solicitado')
        }
      } catch {
        // Keep the live view responsive; the next interval retries the read.
      } finally {
        checking = false
      }
    }

    void refreshRunStatus()
    const timer = window.setInterval(() => void refreshRunStatus(), 1_500)
    return () => {
      cancelled = true
      window.clearInterval(timer)
    }
  }, [runId, result])

  async function handleStart() {
    if (action || isExecutionLocked(result)) return
    const rows = clampMazeDimension(mazeRows)
    const columns = clampMazeDimension(mazeColumns)
    const nextDimensions = { rows, columns, label: `${columns} × ${rows}` }
    dimensionsRef.current = nextDimensions
    runIdRef.current = null
    setRunId(null)
    setRoute([])
    setTelemetry({})
    setStartedAt(null)
    setElapsed(0)
    setMazeRows(rows)
    setMazeColumns(columns)
    setDimensions(nextDimensions)
    setAction('starting')
    setResult('Início solicitado')
    setNotice('Solicitando início…')
    setInterruptRetryAllowed(false)
    try {
      const run = await startRun({ mazeRows: rows, mazeColumns: columns, startPosition })
      runIdRef.current = run.id
      setRunId(run.id)
    } catch (error) {
      setResult('Falhou')
      setNotice(error instanceof Error ? error.message : 'Não foi possível iniciar a execução.')
      setAction(null)
    }
  }

  async function handleInterrupt() {
    if (action || !runId || (result !== 'Em andamento' && result !== 'Interrupção solicitada')) return
    setAction('interrupting')
    setResult('Interrupção solicitada')
    setNotice('Solicitando interrupção…')
    setInterruptRetryAllowed(false)
    try {
      const updated = await interruptRun(runId)
      if (updated.status === 'INTERRUPTED') {
        setResult('Interrompido')
        setNotice('')
        setAction(null)
      }
    } catch (error) {
      setNotice(`${error instanceof Error ? error.message : 'Não foi possível interromper.'} Tente novamente.`)
      setAction(null)
      setResult('Em andamento')
      setInterruptRetryAllowed(true)
    }
  }

  function handleRowsChange(nextRows: number) {
    const rows = clampMazeDimension(nextRows)
    setMazeRows(rows)
    setDimensions({ rows, columns: mazeColumns, label: `${mazeColumns} × ${rows}` })
    setRoute([])
  }

  function handleColumnsChange(nextColumns: number) {
    const columns = clampMazeDimension(nextColumns)
    setMazeColumns(columns)
    setDimensions({ rows: mazeRows, columns, label: `${columns} × ${mazeRows}` })
    setRoute([])
  }

  function handleStartPositionChange(nextStartPosition: StartPosition) {
    setStartPosition(nextStartPosition)
    setRoute([])
  }

  function prepareMap() {
    const rows = clampMazeDimension(mazeRows)
    const columns = clampMazeDimension(mazeColumns)
    setMazeRows(rows)
    setMazeColumns(columns)
    setDimensions({ rows, columns, label: `${columns} × ${rows}` })
    setRoute([])
  }

  const robotMoving = result === 'Em andamento' || result === 'Interrupção solicitada'
  const goalCell = getGoalCell(dimensions.rows, dimensions.columns, startPosition)
  const goalReached = result === 'Sucesso' || route.some((cell) => cell.row === goalCell.row && cell.column === goalCell.column)

  return (
    <main className="min-h-screen bg-page text-text">
      <AppHeader />

      <div className="mx-auto w-full max-w-[1440px] px-[5vw] pb-8 pt-[38px] max-[960px]:px-6 max-[700px]:px-4 max-[700px]:pb-5 max-[700px]:pt-7" id="monitor">
        <section className="mb-6 flex items-end justify-between max-[700px]:mb-[18px] max-[700px]:flex-col max-[700px]:items-start max-[700px]:gap-[14px]">
          <div>
            <h1 className="m-0 font-display text-[clamp(36px,4vw,48px)] leading-[.98] font-semibold tracking-[.005em] text-text">Monitoramento</h1>
            <p className="mt-2 mb-0 text-body text-secondary">
              Acompanhe o percurso e os dados da execução em tempo real.
            </p>
          </div>
          <ConnectionStatus status={status} clock={clock} />
        </section>

        <ExecutionControls
          mazeRows={mazeRows}
          mazeColumns={mazeColumns}
          startPosition={startPosition}
          result={result}
          robotMoving={robotMoving}
          onRowsChange={handleRowsChange}
          onColumnsChange={handleColumnsChange}
          onStartPositionChange={handleStartPositionChange}
          onPrepare={prepareMap}
          busy={action !== null}
          executionLocked={isExecutionLocked(result)}
          canInterrupt={(result === 'Em andamento' || result === 'Interrupção solicitada' || interruptRetryAllowed) && runId !== null}
          onStart={handleStart}
          onInterrupt={handleInterrupt}
          notice={notice}
        />

        {!connected && (
          <div className="mt-[10px] mb-[-4px] flex items-start gap-2 text-small leading-[1.45] text-secondary">
            <span className="mt-[.4em] size-1.5 shrink-0 rounded-full bg-amber" />
            <span className="min-w-0">
              Aguardando conexão com o WebSocket. Os indicadores serão preenchidos quando a
              telemetria chegar.
            </span>
          </div>
        )}

        <div className="mt-4 grid grid-cols-[minmax(0,1fr)_280px] items-start gap-4 max-[960px]:grid-cols-[minmax(0,1fr)_250px] max-[960px]:gap-3 max-[700px]:grid-cols-1">
          <MazePanel
            dimensions={dimensions}
            startPosition={startPosition}
            route={route}
            distanceTravelledMeters={telemetry.distanceTravelledMeters}
          />
          <ExecutionMetrics
            telemetry={telemetry}
            clock={clock}
            elapsedSeconds={elapsed}
            result={result}
            dimensions={dimensions}
            startPosition={startPosition}
            robotMoving={robotMoving}
            goalReached={goalReached}
          />
        </div>

      </div>
    </main>
  )
}

function isExecutionLocked(result: RunResult) {
  return result === 'Início solicitado' || result === 'Em andamento' || result === 'Interrupção solicitada'
}

function isPositionInsideMaze(
  position: Position | undefined,
  dimensions: MazeDimensions,
): position is Position {
  return (
    position !== undefined &&
    Number.isInteger(position.row) &&
    Number.isInteger(position.column) &&
    position.row >= 0 &&
    position.column >= 0 &&
    position.row < dimensions.rows &&
    position.column < dimensions.columns
  )
}

export default App
