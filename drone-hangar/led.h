#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
  private:
    int pin;
  public:
    Led(int p);
    void init();
    void on();
    void off();
    void blink();
    bool isOn();
};

#endif