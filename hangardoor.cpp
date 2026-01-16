#include "hangardoor.h"

// --- HANGAR DOOR IMPLEMENTATION ---
HangarDoor::HangarDoor(int p) : pin(p) {}

void HangarDoor::init() {
  servo.attach(pin);
  close(); // Start closed
}

void HangarDoor::open() {
  servo.write(OPEN_ANGLE);
}

void HangarDoor::close() {
  servo.write(CLOSED_ANGLE);
}