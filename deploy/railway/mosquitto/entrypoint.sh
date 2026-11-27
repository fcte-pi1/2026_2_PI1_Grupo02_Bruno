#!/bin/sh
set -e
: "${MQTT_USERNAME:?Defina MQTT_USERNAME nas variaveis do servico}"
: "${MQTT_PASSWORD:?Defina MQTT_PASSWORD nas variaveis do servico}"
mkdir -p /mosquitto/passwd
rm -f /mosquitto/passwd/passwordfile
mosquitto_passwd -c -b /mosquitto/passwd/passwordfile "$MQTT_USERNAME" "$MQTT_PASSWORD"
chown -R mosquitto:mosquitto /mosquitto/passwd
chmod 0700 /mosquitto/passwd
chmod 0600 /mosquitto/passwd/passwordfile
exec mosquitto -c /mosquitto/config/mosquitto.conf