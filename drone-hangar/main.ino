#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "constants.h"
#include "scheduler.h"
#include "context.h"
#include "sensingtask.h"
#include "logictask.h"
#include "commtask.h"

Display* myDisplay;
Sonar* mySonar;
TempSensor* myTemp;
HangarDoor* myDoor;
PirSensor* myRadar;
Led* led1;
Led* led2;
Led* led3;
Button* btnReset;

Scheduler sched;
Context* context;

SensingTask* tSensing;
LogicTask* tLogic;
CommTask* tComm;

void setup() {
  Serial.begin(115200);
  
  myDisplay = new Display(0x27, 20, 4);
  mySonar = new Sonar(TRIG_PIN, ECHO_PIN);
  myTemp = new TempSensor(TEMP_PIN);
  myDoor = new HangarDoor(SERVO_PIN);
  myRadar = new PirSensor(RADAR_PIN);
  led1 = new Led(LED_PIN1);
  led2 = new Led(LED_PIN2);
  led3 = new Led(LED_PIN3);
  btnReset = new Button(BUTTON_PIN);
  context = new Context();

  sched.init(50); // Tick base: 50ms
  
  tSensing = new SensingTask(mySonar, myTemp, myRadar, context);
  tSensing->init(50);
  sched.addTask(tSensing);

  tLogic = new LogicTask(context, myDisplay, myDoor, led1, led2, led3, btnReset);
  tLogic->init(100);
  sched.addTask(tLogic);

  tComm = new CommTask(context);
  tComm->init(200);
  sched.addTask(tComm);
}

void loop() {
  sched.schedule();
}