#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>
#include <Servo.h>

// --- TIMER CLASS ---
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