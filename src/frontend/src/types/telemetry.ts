import type { Position } from './maze'

export type Telemetry = {
  position?: Position
  distanceTravelledMeters?: number
  currentSpeedMetersPerSecond?: number
  averageSpeedMetersPerSecond?: number
  batteryVoltageVolts?: number
  currentMilliAmps?: number
  currentPowerWatts?: number
  batteryStatus?: string
  chargeConsumedMilliampHours?: number
  energyConsumedWattHours?: number
}

export type RunResult =
  | 'Em andamento'
  | 'Sucesso'
  | 'Interrompido'
  | 'Aguardando'
  | 'Não concluído'
  | 'Resultado indisponível'
  | 'Início solicitado'
  | 'Interrupção solicitada'
  | 'Falhou'
