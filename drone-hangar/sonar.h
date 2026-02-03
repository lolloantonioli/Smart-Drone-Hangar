#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>

class Sonar {
  private:
    int trigPin, echoPin;
    float vs; 
  public:
    Sonar(int trig, int echo);
    float getDistanceCM();
};


#endif
