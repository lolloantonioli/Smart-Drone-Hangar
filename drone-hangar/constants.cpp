#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define LED_PIN1 2
#define LED_PIN2 3
#define LED_PIN3 4
#define SERVO_PIN 5
#define BUTTON_PIN 6
#define RADAR_PIN 11
#define ECHO_PIN 12
#define TRIG_PIN 13
#define TEMP_PIN A3

const float TEMP1 = 25.0;
const int D1 = 100; 
const int D2 = 100; 

const unsigned long T1 = 2000;
const unsigned long T2 = 2000;
const unsigned long T3 = 3000;
const unsigned long T4 = 5000;

enum State {
    INSIDE,
    TAKEOFF,
    DRONEOUT,
    LANDING,
    PREALARM,
    ALARM
};