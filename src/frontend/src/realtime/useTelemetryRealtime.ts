import { Client, ReconnectionTimeMode } from '@stomp/stompjs'
import { useEffect } from 'react'

const REALTIME_DESTINATIONS = [
  '/topic/run-started',
  '/topic/telemetry',
  '/topic/run-finished',
  '/topic/run-interrupted',
]

export function useTelemetryRealtime() {
  useEffect(() => {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:'
    const client = new Client({
      brokerURL: `${protocol}//${window.location.host}/ws`,
      connectionTimeout: 8_000,
      reconnectDelay: 1_000,
      reconnectTimeMode: ReconnectionTimeMode.EXPONENTIAL,
      maxReconnectDelay: 30_000,
      heartbeatIncoming: 10_000,
      heartbeatOutgoing: 10_000,
      onConnect: () => {
        console.info('[realtime] connected; subscribing to', REALTIME_DESTINATIONS)
        REALTIME_DESTINATIONS.forEach((destination) => {
          client.subscribe(destination, (message) => {
            try {
              const payload: unknown = JSON.parse(message.body)
              console.log(`[realtime] message received from ${destination}:`, payload)
            } catch (error) {
              console.error(`[realtime] invalid JSON from ${destination}:`, message.body, error)
            }
          })
        })
      },
      onWebSocketClose: (event) => {
        console.warn('[realtime] connection closed; reconnect will be attempted', event)
      },
      onWebSocketError: (event) => {
        console.error('[realtime] WebSocket error:', event)
      },
      onStompError: (frame) => {
        console.error('[realtime] broker error:', frame.headers['message'], frame.body)
      },
    })

    client.activate()
    return () => {
      void client.deactivate()
    }
  }, [])
}
