#include "logictask.h"
#include "constants.h"

LogicTask::LogicTask(Context* ctx, Display* d, HangarDoor* door, Led* l1, Led* l2, Led* l3, Button* btn) {
    this->context = ctx;
    this->display = d;
    this->door = door;
    this->l1 = l1;
    this->l2 = l2;
    this->l3 = l3;
    this->resetBtn = btn;
    
    this->stateTimer = new Timer();
    this->tempTimer = new Timer();
    
    currentState = INSIDE;
    previousState = INSIDE;
}

void LogicTask::init(int period) {
    Task::init(period);
    display->init();
    door->init();
    l1->init();
    l2->init();
    l3->init();
    resetBtn->init();
    
    display->showInside();
    l1->on();
    
    context->setState(currentState);
}

void LogicTask::checkTempAndAlarm(float currentTemp) {
    if (currentTemp >= TEMP1 && !tempTimer->isActive()) {
        tempTimer->start();
    } else if (currentTemp < TEMP1) {
        tempTimer->reset();
    } else if (tempTimer->isExpired(T3)) {
        if(currentState == INSIDE) {
            door->close();
        }
        previousState = currentState;
        tempTimer->start();
        currentState = PREALARM;
    }
}

void LogicTask::tick() {
    float currentDist = context->getDist();
    float currentTemp = context->getTemp();
    bool currentMotion = context->isMotion();
    bool cmdTakeoff = context->hasTakeoffCmd();
    bool cmdLand = context->hasLandCmd();

    switch (currentState) {
        case INSIDE:
            if (cmdTakeoff) {
                context->setTakeoffCmd(false);
                l1->off();
                door->open();
                display->clear();
                display->showTakeOff();
                stateTimer->reset();
                currentState = TAKEOFF;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case TAKEOFF:            
            l2->blink();
            if (currentDist >= D1 && !stateTimer->isActive()) {
                stateTimer->start();
            } else if (currentDist < D1) {
                stateTimer->reset();
            } else if (stateTimer->isExpired(T1)) {
                door->close();
                display->clear();
                display->showDroneOut();
                l2->off();
                context->setLandCmd(false);
                currentState = DRONEOUT;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case DRONEOUT:
            if (cmdLand && currentMotion) {
                context->setLandCmd(false);
                door->open();
                display->clear();
                display->showLanding();
                stateTimer->reset();
                currentState = LANDING;
            }
            break;

        case LANDING:
            l2->blink();
            if (currentDist <= D2 && !stateTimer->isActive()) {
                stateTimer->start();
            } else if (currentDist > D2) {
                stateTimer->reset();
            } else if (stateTimer->isExpired(T2)) {
                door->close();
                display->clear();
                display->showInside();
                l1->on();
                l2->off();
                context->setTakeoffCmd(false);
                currentState = INSIDE;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case PREALARM:
            if (previousState == LANDING) {
                l2->blink();
                if (currentDist <= D2 && !stateTimer->isActive()) {
                    stateTimer->start();
                } else if (currentDist > D2) {
                    stateTimer->reset();
                } else if (stateTimer->isExpired(T2)) {
                    door->close();
                    previousState = INSIDE;
                    l2->off();
                    l1->on();
                }
            } else if (previousState == TAKEOFF) {
                l2->blink();
                if (currentDist >= D1 && !stateTimer->isActive()) {
                    stateTimer->start();
                } else if (currentDist < D1) {
                    stateTimer->reset();
                } else if (stateTimer->isExpired(T1)) {
                    door->close();
                    previousState = DRONEOUT;
                    l2->off();
                }
            }

            if (currentTemp < TEMP1) {
                currentState = previousState;
            } else if (tempTimer->isExpired(T4)) {
                if (previousState == LANDING) {
                    previousState = INSIDE;
                } else if (previousState == TAKEOFF) {
                    previousState = DRONEOUT;
                }

                door->close();
                l3->on();
                l1->off();
                l2->off();
                display->clear();
                display->showAlarm();
                Serial.println("ALARM:ON");
                currentState = ALARM;
            }
            break;

        case ALARM:
            if (resetBtn->isPressed()) {
                l3->off();
                tempTimer->reset();
                display->clear();
                if (previousState == INSIDE) {
                    l1->on(); display->showInside();
                } else {
                    display->showDroneOut();
                }
                currentState = previousState;
            }
            break;
    }

    context->setState(currentState);
}