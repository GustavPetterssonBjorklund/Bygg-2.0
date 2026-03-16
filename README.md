# Bygg 2.0 Dual Motor MQTT Controller

This repository contains:

- A **Svelte + Tailwind** web UI (`/src`) with a black/purple theme.
- An **ESP8266 PlatformIO firmware** (`/firmware`) that controls two DC motors via a motor bridge and listens to MQTT messages.
- A shared root `.env` file used by both projects.

## Shared environment setup

1. Copy `.env.example` to `.env`.
2. Update broker and Wi-Fi values.

The firmware uses `firmware/scripts/load_env.py` to generate `firmware/include/mqtt_config.h` from the root `.env`.

## Web app

```bash
npm install
npm run dev
```

## Firmware

```bash
cd firmware
pio run
pio run -t upload
```

## MQTT payload format

Published topic: `${VITE_MQTT_COMMAND_TOPIC}/set`

```json
{
  "motorId": "motorA",
  "speed": 120
}
```

`motorId` supports `motorA` and `motorB`, speed range is `-255..255`.
