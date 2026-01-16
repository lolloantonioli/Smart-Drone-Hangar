#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>
#include <Servo.h>

// --- TEMP SENSOR CLASS ---
class TempSensor {
  private:
    int pin;
  public:
    TempSensor(int p);
    float getTemp();
};

#endif
