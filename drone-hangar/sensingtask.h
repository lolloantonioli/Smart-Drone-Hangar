#ifndef SENSING_TASK_H
#define SENSING_TASK_H

#include "task.h"
#include "context.h"
#include "sonar.h"
#include "tempsensor.h"
#include "pirsensor.h"

class SensingTask : public Task {
  private:
    Sonar* sonar;
    TempSensor* temp;
    PirSensor* pir;
    Context* context;

  public:
    SensingTask(Sonar* s, TempSensor* t, PirSensor* p, Context* ctx);
    void init(int period);
    void tick();
};

#endif