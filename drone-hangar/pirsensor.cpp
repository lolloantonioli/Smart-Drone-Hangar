#include "pirsensor.h"

PirSensor::PirSensor(int p) : pin(p) {}

void PirSensor::init() {
    pinMode(pin, INPUT);
}

bool PirSensor::isDetected() {
    return digitalRead(pin) == HIGH;
}