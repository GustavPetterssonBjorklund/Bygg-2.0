import os
from pathlib import Path

Import('env')

project_dir = Path(env['PROJECT_DIR'])
root_env_path = project_dir.parent / '.env'
output_file = project_dir / 'include' / 'mqtt_config.h'

required = [
    'MQTT_BROKER_HOST',
    'MQTT_BROKER_PORT',
    'MQTT_CLIENT_ID',
    'MQTT_TOPIC_COMMAND',
    'WIFI_SSID',
    'WIFI_PASSWORD',
]

def parse_env(path):
    parsed = {}
    if not path.exists():
        raise RuntimeError(f'Missing shared .env file at {path}')

    for raw in path.read_text(encoding='utf-8').splitlines():
        line = raw.strip()
        if not line or line.startswith('#') or '=' not in line:
            continue
        key, value = line.split('=', 1)
        parsed[key.strip()] = value.strip().strip('"').strip("'")
    return parsed

def write_header(config):
    output_file.parent.mkdir(parents=True, exist_ok=True)
    lines = [
        '#pragma once',
        '',
        f'#define WIFI_SSID "{config["WIFI_SSID"]}"',
        f'#define WIFI_PASSWORD "{config["WIFI_PASSWORD"]}"',
        f'#define MQTT_BROKER_HOST "{config["MQTT_BROKER_HOST"]}"',
        f'#define MQTT_BROKER_PORT {config["MQTT_BROKER_PORT"]}',
        f'#define MQTT_CLIENT_ID "{config["MQTT_CLIENT_ID"]}"',
        f'#define MQTT_TOPIC_COMMAND "{config["MQTT_TOPIC_COMMAND"]}"',
        '',
    ]
    output_file.write_text('\n'.join(lines), encoding='utf-8')

config = parse_env(root_env_path)
missing = [key for key in required if key not in config]
if missing:
    raise RuntimeError('Missing env keys: ' + ', '.join(missing))

write_header(config)
print(f'Generated {output_file}')
