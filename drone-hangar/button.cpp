#include "button.h"

Button::Button(int p) : pin(p) {}

void Button::init() {
    pinMode(pin, INPUT);
}

bool Button::isPressed() {
    return digitalRead(pin) == HIGH;
}