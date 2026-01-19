#include "led.h"

Led::Led(int p) : pin(p) {}

void Led::init() {
    pinMode(pin, OUTPUT);
    off();
}

void Led::on() {
    digitalWrite(pin, HIGH);
}

void Led::off() {
    digitalWrite(pin, LOW);
}

void Led::blink() {
    if ((millis() % 1000) < 500) {
        digitalWrite(pin, HIGH);
    } else {
        digitalWrite(pin, LOW);
    }
}

bool Led::isOn() {
    return digitalRead(pin) == HIGH;
}