#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "MotorController.h"
#include "mqtt_config.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// D1-D6 are easy-to-route pins for L298N/L9110 style bridges.
MotorController motorA(D1, D2, D3);
MotorController motorB(D5, D6, D7);

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(350);
  }
}

void setMotorSpeed(const char* motorId, int speed) {
  if (strcmp(motorId, "motorA") == 0) {
    motorA.setSpeed(speed);
    return;
  }

  if (strcmp(motorId, "motorB") == 0) {
    motorB.setSpeed(speed);
  }
}

void handleCommand(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<96> doc;
  auto err = deserializeJson(doc, payload, length);
  if (err) {
    return;
  }

  const char* motorId = doc["motorId"] | "";
  int speed = doc["speed"] | 0;
  setMotorSpeed(motorId, speed);
}

void ensureMqttConnection() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      mqttClient.subscribe(MQTT_TOPIC_COMMAND);
    } else {
      delay(1500);
    }
  }
}

void setup() {
  motorA.begin();
  motorB.begin();

  connectWifi();

  mqttClient.setServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
  mqttClient.setCallback(handleCommand);
}

void loop() {
  if (!mqttClient.connected()) {
    ensureMqttConnection();
  }

  mqttClient.loop();
}
