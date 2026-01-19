#include "sensingtask.h"

SensingTask::SensingTask(Sonar* s, TempSensor* t, PirSensor* p, Context* ctx) {
    this->sonar = s;
    this->temp = t;
    this->pir = p;
    this->context = ctx;
}

void SensingTask::init(int period) {
    Task::init(period);
    pir->init(); 
}

void SensingTask::tick() {
    float d = sonar->getDistanceCM();
    float t = temp->getTemp();
    bool m = pir->isDetected();
    context->setSensors(d, t, m);
}