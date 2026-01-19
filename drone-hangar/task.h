#ifndef TASK_H
#define TASK_H

class Task {
    int myPeriod;
    int timeElapsed;
    bool active;
    
public:
    virtual void init(int period) {
        myPeriod = period;  
        timeElapsed = 0;
        active = true;
    }

    virtual void tick() = 0;

    bool updateAndCheckTime(int basePeriod) {
        timeElapsed += basePeriod;
        if (timeElapsed >= myPeriod) {
            timeElapsed = 0;
            return true;
        }
        return false;
    }
    
    void setActive(bool active) {
        this->active = active;
    }
    
    bool isActive() {
        return active;
    }
};

#endif