#pragma once

#include <Arduino.h>

class MotorController {
 public:
  MotorController(uint8_t in1Pin, uint8_t in2Pin, uint8_t pwmPin);
  void begin();
  void setSpeed(int speed);

 private:
  uint8_t _in1Pin;
  uint8_t _in2Pin;
  uint8_t _pwmPin;
};
