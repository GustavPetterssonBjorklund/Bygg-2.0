import { connect } from 'mqtt';

const APP_TOPIC_NAMESPACE = import.meta.env.VITE_MQTT_COMMAND_TOPIC ?? 'bygg/motors';

export function createMotorClient() {
  const brokerUrl = import.meta.env.VITE_MQTT_BROKER_URL;

  if (!brokerUrl) {
    throw new Error('VITE_MQTT_BROKER_URL is not defined. Check your .env file.');
  }

  const client = connect(brokerUrl, {
    username: import.meta.env.VITE_MQTT_USERNAME || undefined,
    password: import.meta.env.VITE_MQTT_PASSWORD || undefined,
    reconnectPeriod: 1500,
    keepalive: 45
  });

  return {
    on(eventName, callback) {
      client.on(eventName, callback);
    },
    publishMotorSpeed(motorId, speed) {
      const clamped = Math.max(-255, Math.min(255, Number(speed) || 0));
      const payload = JSON.stringify({ motorId, speed: clamped });
      client.publish(`${APP_TOPIC_NAMESPACE}/set`, payload, { qos: 1 });
    },
    disconnect() {
      client.end(true);
    }
  };
}
