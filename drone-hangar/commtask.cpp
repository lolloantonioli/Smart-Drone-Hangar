#include "commtask.h"
#include "constants.h"

CommTask::CommTask(Context* ctx) {
    this->context = ctx;
    this->lastSentState = ALARM; 
}

void CommTask::init(int period) {
    Task::init(period);
}

void CommTask::tick() {
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        if (msg.equals("CMD:TAKEOFF")) {
            context->setTakeoffCmd(true);
        } else if (msg.equals("CMD:LAND")) {
            context->setLandCmd(true);
        }
    }
    State currentState = context->getState();
    if (currentState != lastSentState) {
        Serial.print("STATE:");
        switch (currentState) {
            case INSIDE: Serial.println("INSIDE"); break;
            case TAKEOFF: Serial.println("TAKEOFF"); break;
            case DRONEOUT: Serial.println("DRONEOUT"); break;
            case LANDING: Serial.println("LANDING"); break;
            case PREALARM: Serial.println("PREALARM"); break;
            case ALARM: Serial.println("ALARM"); break;
        }
        lastSentState = currentState;
    }

    Serial.print("DIST:");
    Serial.println(context->getDist());
    Serial.print("TEMP:");
    Serial.println(context->getTemp());
    Serial.print("MOV:");
    Serial.println(context->isMotion() ? "SI" : "NO");
}