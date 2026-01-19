#ifndef TMP_SENSOR_H
#define TMP_SENSOR_H

#include <Arduino.h>
#include <Servo.h>

class TempSensor {
  private:
    int pin;
  public:
    TempSensor(int p);
    float getTemp();
};

#endif
