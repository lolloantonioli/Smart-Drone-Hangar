#ifndef COMM_TASK_H
#define COMM_TASK_H

#include "task.h"
#include "context.h"
#include <Arduino.h>

class CommTask : public Task {
  private:
    Context* context;
    int lastSentState;
    
  public:
    CommTask(Context* ctx);
    void init(int period);
    void tick();
};

#endif