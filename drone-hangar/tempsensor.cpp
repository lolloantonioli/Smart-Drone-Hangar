#include "tempsensor.h"

TempSensor::TempSensor(int p) : pin(p) {}

float TempSensor::getTemp() {
  int value = analogRead(pin);
  float volts = value * 0.00488;
  return volts / 0.01;
}