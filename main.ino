#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#include "hangardoor.h"
#include "sonar.h"
#include "tempsensor.h"
#include "timer.h"
#include "constants.h"
#include "hangarcontroller.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
Sonar mySonar(TRIG_PIN, ECHO_PIN);
TempSensor myTemp(TEMP_PIN);
HangarDoor myDoor(SERVO_PIN);
HangarController hangar(lcd, mySonar, myTemp, myDoor);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RADAR_PIN, INPUT);
  hangar.begin();
}

void loop() {
  hangar.update();
}