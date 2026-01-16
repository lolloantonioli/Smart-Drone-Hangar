#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>
#include <Servo.h>

// --- SONAR CLASS ---
class Sonar {
  private:
    int trigPin, echoPin;
    float vs; 
  public:
    Sonar(int trig, int echo);
    float getDistanceCM();
};

#endif