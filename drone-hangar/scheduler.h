#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

#define MAX_TASKS 10

class Scheduler {
    int basePeriod;
    int nTasks;
    Task* taskList[MAX_TASKS];
    
public:
    void init(int basePeriod);
    bool addTask(Task* task);
    void schedule();
};

#endif