export type MazeSize = 'GRID_4X4' | 'GRID_8X4' | 'GRID_12X4'

export type StartPosition = 'bottom_left' | 'bottom_right'

export const MIN_MAZE_DIMENSION = 1
export const MAX_MAZE_DIMENSION = 18

export type MazeDimensions = {
  label: string
  rows: number
  columns: number
}

export type Position = {
  row: number
  column: number
  heading?: string
}

export const mazeOptions: Record<MazeSize, MazeDimensions> = {
  GRID_4X4: { label: '4 × 4', rows: 4, columns: 4 },
  GRID_8X4: { label: '8 × 4', rows: 4, columns: 8 },
  GRID_12X4: { label: '12 × 4', rows: 4, columns: 12 },
}

export const startPositionLabels: Record<StartPosition, string> = {
  bottom_left: 'Inferior esquerdo (bottom_left)',
  bottom_right: 'Inferior direito (bottom_right)',
}

export const headingLabels: Record<string, string> = {
  NORTH: 'Norte',
  EAST: 'Leste',
  SOUTH: 'Sul',
  WEST: 'Oeste',
}

export function clampMazeDimension(value: number) {
  if (!Number.isFinite(value)) return 4
  return Math.max(MIN_MAZE_DIMENSION, Math.min(MAX_MAZE_DIMENSION, Math.round(value)))
}

export function getStartCell(rows: number, columns: number, startPosition: StartPosition): Position {
  return startPosition === 'bottom_right'
    ? { row: rows - 1, column: columns - 1, heading: 'WEST' }
    : { row: rows - 1, column: 0, heading: 'EAST' }
}

export function getGoalCell(_rows: number, columns: number, startPosition: StartPosition): Position {
  return startPosition === 'bottom_right'
    ? { row: 0, column: 0 }
    : { row: 0, column: columns - 1 }
}
