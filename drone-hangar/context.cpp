#include "context.h"

Context::Context() {
    dist = 0.0;
    temp = 0.0;
    motion = false;
    takeoffCmd = false;
    landCmd = false;
    currentState = INSIDE;
}

void Context::setSensors(float d, float t, bool m) {
    this->dist = d;
    this->temp = t;
    this->motion = m;
}

void Context::setTakeoffCmd(bool cmd) {
    this->takeoffCmd = cmd;
}

void Context::setLandCmd(bool cmd) {
    this->landCmd = cmd;
}

void Context::setState(State s) {
    this->currentState = s;
}


float Context::getDist() {
    return dist;
}

float Context::getTemp() {
    return temp;
}

bool Context::isMotion() {
    return motion;
}

bool Context::hasTakeoffCmd() {
    return takeoffCmd;
}

bool Context::hasLandCmd() {
    return landCmd;
}

State Context::getState() {
    return currentState;
}