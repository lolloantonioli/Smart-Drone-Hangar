#include "display.h"

Display::Display(uint8_t addr, uint8_t cols, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(addr, cols, rows);
}

void Display::init() {
    lcd->init();
    lcd->backlight();
    clear();
}

void Display::clear() {
    lcd->clear();
}

void Display::showInside() {
    lcd->setCursor(0, 0);
    lcd->print("DRONE INSIDE");
}

void Display::showTakeOff() {
    lcd->setCursor(0, 0);
    lcd->print("TAKE OFF");
}

void Display::showDroneOut() {
    lcd->setCursor(0, 0);
    lcd->print("DRONE OUT");
}

void Display::showLanding() {
    lcd->setCursor(0, 0);
    lcd->print("LANDING");
}

void Display::showAlarm() {
    lcd->setCursor(0, 0);
    lcd->print("ALARM");
}