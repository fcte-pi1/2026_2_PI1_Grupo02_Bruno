import { MicromouseIcon } from './MicromouseIcon'
import { getGoalCell, getStartCell, type StartPosition } from '../types/maze'

type Cell = {
  row: number
  column: number
  heading?: string
}

type Props = {
  rows: number
  columns: number
  route: Cell[]
  startPosition?: StartPosition
  compact?: boolean
}

export function MazeBoard({ rows, columns, route, startPosition = 'bottom_left', compact = false }: Props) {
  const validRoute = route.filter(
    ({ row, column }) => row >= 0 && column >= 0 && row < rows && column < columns,
  )
  const visitedCells = new Set(validRoute.map(({ row, column }) => `${row},${column}`))
  const configuredStart = getStartCell(rows, columns, startPosition)
  const goal = getGoalCell(rows, columns, startPosition)
  const denseGrid = rows > 10 || columns > 10

  const getCenter = ({ row, column }: Cell) => ({
    x: ((column + 0.5) / columns) * 100,
    y: ((row + 0.5) / rows) * 100,
  })
  const points = validRoute
    .map((cell) => {
      const { x, y } = getCenter(cell)
      return `${x},${y}`
    })
    .join(' ')
  const start = validRoute[0] ?? configuredStart
  const current = validRoute[validRoute.length - 1]
  const isGoalReached = visitedCells.has(`${goal.row},${goal.column}`)

  return (
    <div
      className={compact
        ? 'grid min-h-0 place-items-center overflow-hidden rounded-[2px] border border-[#353a35] bg-surface p-2.5'
        : 'grid min-h-[300px] place-items-center overflow-hidden rounded-[2px] border border-[#353a35] bg-[#191c1a] p-5 max-[960px]:min-h-[260px] max-[960px]:p-[15px] max-[700px]:min-h-[210px] max-[700px]:px-2'}
      aria-label={`Labirinto com ${rows} linhas e ${columns} colunas; ${visitedCells.size} células percorridas`}
    >
      <div
        className={compact
          ? 'relative grid w-full gap-[2px] rounded-[2px] border border-[#50564d] bg-[#262b26] p-2'
          : `relative grid max-h-[480px] w-full max-w-[790px] ${denseGrid ? 'gap-[1.5px] p-1.5' : 'gap-[3px] p-2'} rounded-[2px] border border-[#50564d] bg-[#262b26] max-[960px]:gap-0.5 max-[960px]:p-1.5`}
        style={{
          gridTemplateColumns: `repeat(${columns}, minmax(0, 1fr))`,
          aspectRatio: `${columns} / ${rows}`,
        }}
      >
        {Array.from({ length: rows * columns }, (_, index) => {
          const cellRow = Math.floor(index / columns)
          const cellColumn = index % columns
          const key = `${cellRow},${cellColumn}`
          const isVisited = visitedCells.has(key)
          const isCurrent = current?.row === cellRow && current?.column === cellColumn
          const isGoalCell = goal.row === cellRow && goal.column === cellColumn

          const cellStyle = isCurrent
            ? 'border-[#8bc973] bg-[#36643a]'
            : isVisited
              ? 'border-[#548c55] bg-[#294d2d]'
              : isGoalCell
                ? 'border-[#b38c4e] bg-[#2f2a1f]'
                : 'border-[#3a403a] bg-[#222622]'

          return (
            <div
              className={`min-h-0 min-w-0 rounded-[1px] border transition-colors duration-150 ${cellStyle}`}
              key={key}
              title={`Linha ${cellRow}, Coluna ${cellColumn}${isVisited ? ' (percorrida)' : ''}${isGoalCell ? ' (objetivo)' : ''}`}
            />
          )
        })}

        <svg
          className={`pointer-events-none absolute ${denseGrid && !compact ? 'inset-1.5 h-[calc(100%-12px)] w-[calc(100%-12px)]' : 'inset-2 h-[calc(100%-16px)] w-[calc(100%-16px)]'} overflow-visible max-[960px]:inset-1.5 max-[960px]:h-[calc(100%-12px)] max-[960px]:w-[calc(100%-12px)]`}
          viewBox="0 0 100 100"
          preserveAspectRatio="none"
          aria-hidden="true"
        >
          {validRoute.length > 1 && (
            <polyline
              points={points}
              fill="none"
              stroke="#a6c58a"
              strokeWidth="1.25"
              strokeLinecap="round"
              strokeLinejoin="round"
              vectorEffect="non-scaling-stroke"
            />
          )}
        </svg>

        <div className={`pointer-events-none absolute ${denseGrid && !compact ? 'inset-1.5' : 'inset-2'} max-[960px]:inset-1.5`}>
          <span
            className={`absolute -translate-x-1/2 -translate-y-1/2 rounded-[2px] border ${
              isGoalReached
                ? 'border-[#a6c58a] bg-[#a6c58a]/30'
                : 'border-amber/80 bg-amber/15'
            } ${denseGrid ? 'size-[9px]' : 'size-[13px]'}`}
            style={{ left: `${getCenter(goal).x}%`, top: `${getCenter(goal).y}%` }}
            aria-hidden="true"
          />
          {start && (
            <span
              className={`absolute -translate-x-1/2 -translate-y-1/2 rounded-full border-2 border-[#252a25] bg-[#c2beb0] shadow-[0_0_0_1px_#c2beb077] ${denseGrid ? 'size-[8px]' : 'size-[11px]'}`}
              style={{ left: `${getCenter(start).x}%`, top: `${getCenter(start).y}%` }}
              aria-hidden="true"
            />
          )}
          {current && (
            <span
              className={`absolute aspect-square -translate-x-1/2 -translate-y-1/2 drop-shadow-[0_2px_2px_#000b] ${
                compact
                  ? 'w-[18px]'
                  : denseGrid
                    ? 'w-[clamp(12px,1.5vw,18px)]'
                    : 'w-[clamp(19px,2.8vw,29px)]'
              }`}
              style={{ left: `${getCenter(current).x}%`, top: `${getCenter(current).y}%` }}
              aria-hidden="true"
            >
              <MicromouseIcon heading={current.heading} />
            </span>
          )}
        </div>
      </div>
    </div>
  )
}
