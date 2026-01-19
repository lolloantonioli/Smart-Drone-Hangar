#ifndef LOGIC_TASK_H
#define LOGIC_TASK_H

#include "task.h"
#include "context.h"
#include "display.h"
#include "hangardoor.h"
#include "led.h"
#include "button.h"
#include "timer.h"
#include "constants.h"

class LogicTask : public Task {
  private:
    Context* context;
    Display* display;
    HangarDoor* door;
    Led* l1;
    Led* l2;
    Led* l3;
    Button* resetBtn;

    State currentState;
    State previousState;
    Timer* stateTimer; 
    Timer* tempTimer;  

    void checkTempAndAlarm(float currentTemp);

  public:
    LogicTask(Context* ctx, Display* d, HangarDoor* door, Led* l1, Led* l2, Led* l3, Button* btn);
    void init(int period);
    void tick();
};

#endif