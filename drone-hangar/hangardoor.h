#ifndef HANGAR_DOOR_H
#define HANGAR_DOOR_H

#include <Arduino.h>
#include <Servo.h>

class HangarDoor {
  private:
    Servo* servo;
    int pin;
    const int OPEN_ANGLE = 90;
    const int CLOSED_ANGLE = 0;
  public:
    HangarDoor(int p);
    void init();
    void open();
    void close();
};

#endif
