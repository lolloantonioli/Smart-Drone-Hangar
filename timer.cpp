#include "timer.h"

// --- TIMER IMPLEMENTATION ---
Timer::Timer() : active(false) {}

void Timer::start() {
  startTime = millis();
  active = true;
}

void Timer::reset() {
  active = false;
}

bool Timer::isActive() {
  return active;
}

bool Timer::isExpired(unsigned long duration) {
  if (!active) return false;
  return (millis() - startTime >= duration);
}