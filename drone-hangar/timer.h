#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <Servo.h>

class Timer {
  private:
    unsigned long startTime;
    bool active;
  public:
    Timer();
    void start();
    void reset();
    bool isActive();
    bool isExpired(unsigned long duration);
};

#endif