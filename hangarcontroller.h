#ifndef HANGAR_CONTROLLER_H
#define HANGAR_CONTROLLER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "sonar.h"
#include "tempsensor.h"
#include "hangardoor.h"
#include "timer.h"
#include "constants.h"

enum State {
    INSIDE,
    TAKEOFF,
    DRONEOUT,
    LANDING,
    PREALARM,
    ALARM
};

class HangarController {
  private:
    // References
    LiquidCrystal_I2C& lcd;
    Sonar& sonar;
    TempSensor& tempSensor;
    HangarDoor& door;
    
    // State variables
    State currentState;
    State previousState;
    State lastSentState; // Per telemetria
    
    // Serial commands flags
    bool cmdTakeoffReceived;
    bool cmdLandReceived;
    unsigned long lastTelemetryTime;

    // Timers
    Timer stateTimer; // Corrisponde a 'time' nel codice originale
    Timer tempTimer;  // Corrisponde a 'timer' nel codice originale (per T3/T4)

    // Helpers
    void processIncomingSerial();
    void sendTelemetry();
    void checkTempAndAlarm(float currentTemp);

  public:
    HangarController(LiquidCrystal_I2C& l, Sonar& s, TempSensor& t, HangarDoor& d);
    void begin();
    void update();
};

#endif