#include "hangardoor.h"

HangarDoor::HangarDoor(int p) : pin(p) {
    servo = new Servo();
}

void HangarDoor::init() {
  servo->attach(pin);
  close();
}

void HangarDoor::open() {
  servo->write(OPEN_ANGLE);
}

void HangarDoor::close() {
  servo->write(CLOSED_ANGLE);
}