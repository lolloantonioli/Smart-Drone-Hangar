#include "sonar.h"

Sonar::Sonar(int trig, int echo) : trigPin(trig), echoPin(echo) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  vs = 331.5 + 0.6 * 20; 
}

float Sonar::getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  
  float tUS = pulseIn(echoPin, HIGH);
  float t = tUS / 1000.0 / 1000.0 / 2;
  return (t * vs) * 100.0;
}