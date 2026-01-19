#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  private:
    int pin;
  public:
    Button(int p);
    void init();
    bool isPressed();
};

#endif