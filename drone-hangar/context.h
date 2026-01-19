#ifndef CONTEXT_H
#define CONTEXT_H

#include "constants.h"

class Context {
  private:
    float dist;
    float temp;
    bool motion;
    
    bool takeoffCmd;
    bool landCmd;
    
    State currentState;

  public:
    Context();

    void setSensors(float d, float t, bool m);
    void setTakeoffCmd(bool cmd);
    void setLandCmd(bool cmd);
    void setState(State s);

    float getDist();
    float getTemp();
    bool isMotion();
    
    bool hasTakeoffCmd();
    bool hasLandCmd();
    State getState();
};

#endif