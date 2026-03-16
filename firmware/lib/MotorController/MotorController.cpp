#include "MotorController.h"

MotorController::MotorController(uint8_t in1Pin, uint8_t in2Pin, uint8_t pwmPin)
    : _in1Pin(in1Pin), _in2Pin(in2Pin), _pwmPin(pwmPin) {}

void MotorController::begin() {
  pinMode(_in1Pin, OUTPUT);
  pinMode(_in2Pin, OUTPUT);
  pinMode(_pwmPin, OUTPUT);
  analogWriteRange(255);
  setSpeed(0);
}

void MotorController::setSpeed(int speed) {
  int clamped = constrain(speed, -255, 255);
  bool forward = clamped >= 0;
  int pwm = abs(clamped);

  digitalWrite(_in1Pin, forward ? HIGH : LOW);
  digitalWrite(_in2Pin, forward ? LOW : HIGH);
  analogWrite(_pwmPin, pwm);
}
