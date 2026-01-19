#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include <Arduino.h>

class PirSensor {
  private:
    int pin;
  public:
    PirSensor(int p);
    void init();
    bool isDetected();
};

#endif