#include "hangarcontroller.h"

HangarController::HangarController(LiquidCrystal_I2C& l, Sonar& s, TempSensor& t, HangarDoor& d) 
    : lcd(l), sonar(s), tempSensor(t), door(d) 
{
    currentState = INSIDE;
    previousState = INSIDE;
    lastSentState = ALARM; // Dummy value to force first send
    cmdTakeoffReceived = false;
    cmdLandReceived = false;
    lastTelemetryTime = 0;
}

void HangarController::begin() {
    door.init();
    
    // Inizializzazione LCD e LED iniziali
    lcd.setCursor(0, 0);
    lcd.print("DRONE INSIDE");
    
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
}

void HangarController::processIncomingSerial() {
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        if (msg.equals("CMD:TAKEOFF")) {
            cmdTakeoffReceived = true;
        } else if (msg.equals("CMD:LAND")) {
            cmdLandReceived = true;
        }
    }
}

void HangarController::sendTelemetry() {
    // 1. Stato
    if (currentState != lastSentState) {
        Serial.print("STATE:");
        switch (currentState) {
            case INSIDE: Serial.println("INSIDE"); break;
            case TAKEOFF: Serial.println("TAKEOFF"); break;
            case DRONEOUT: Serial.println("DRONEOUT"); break;
            case LANDING: Serial.println("LANDING"); break;
            case PREALARM: Serial.println("PREALARM"); break;
            case ALARM: Serial.println("ALARM"); break;
        }
        lastSentState = currentState;
    }

    // 2. Dati (ogni 500ms)
    if (millis() - lastTelemetryTime > 500) {
        float dist = sonar.getDistanceMeters();
        Serial.print("DIST:");
        Serial.println(dist);
        // Serial.print("TEMP:"); Serial.println(tempSensor.getTemp()); // Opzionale
        lastTelemetryTime = millis();
    }
}

// Logica comune per il controllo temperatura e transizione a PREALARM
void HangarController::checkTempAndAlarm(float currentTemp) {
    if (currentTemp >= TEMP1 && !tempTimer.isActive()) {
        tempTimer.start();
    } else if (currentTemp < TEMP1) {
        tempTimer.reset();
    } else if (tempTimer.isExpired(T3)) {
        // Logica di chiusura porta in caso di allarme da stati specifici
        if(currentState == INSIDE || currentState == TAKEOFF) {
            door.close();
        }
        lcd.setCursor(0, 0);
        // Non stampiamo nulla su LCD qui perché nel codice originale non c'era print specifica nel passaggio a prealarm
        previousState = currentState;
        tempTimer.start(); // Riavvia timer per contare T4
        currentState = PREALARM;
    }
}

void HangarController::update() {
    // 1. Gestione Comandi e Telemetria
    processIncomingSerial();
    sendTelemetry();

    float currentTemp = tempSensor.getTemp();

    switch (currentState) {
        case INSIDE:
            if (cmdTakeoffReceived) {
                cmdTakeoffReceived = false;
                digitalWrite(LED_PIN1, LOW);
                lcd.clear();
                door.open();
                lcd.setCursor(0, 0);
                lcd.print("TAKE OFF");
                stateTimer.reset(); // reset variable time
                currentState = TAKEOFF;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case TAKEOFF:
            Serial.println(currentTemp); // Come da codice originale
            
            // Blink LED2 (Bloccante come da codice originale)
            digitalWrite(LED_PIN2, HIGH);
            delay(500);
            digitalWrite(LED_PIN2, LOW);
            delay(500);

            if (sonar.getDistanceCM() >= D1 && !stateTimer.isActive()) {
                stateTimer.start();
            } else if (sonar.getDistanceCM() < D1) {
                stateTimer.reset();
            } else if (stateTimer.isExpired(T1)) {
                door.close();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("DRONE OUT");
                digitalWrite(LED_PIN2, LOW);
                cmdLandReceived = false; 
                currentState = DRONEOUT;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case DRONEOUT:
            if (cmdLandReceived && digitalRead(RADAR_PIN) == HIGH) {
                cmdLandReceived = false;
                lcd.clear();
                door.open();
                lcd.setCursor(0, 0);
                lcd.print("LANDING");
                stateTimer.reset();
                currentState = LANDING;
            }
            break;

        case LANDING:
            // Blink LED2
            digitalWrite(LED_PIN2, HIGH);
            delay(500);
            digitalWrite(LED_PIN2, LOW);
            delay(500);

            if (sonar.getDistanceCM() <= D2 && !stateTimer.isActive()) {
                stateTimer.start();
            } else if (sonar.getDistanceCM() > D2) {
                stateTimer.reset();
            } else if (stateTimer.isExpired(T2)) {
                door.close();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("DRONE INSIDE");
                digitalWrite(LED_PIN1, HIGH);
                digitalWrite(LED_PIN2, LOW);
                cmdTakeoffReceived = false;
                currentState = INSIDE;
            }
            checkTempAndAlarm(currentTemp);
            break;

        case PREALARM:
            if (previousState == LANDING) {
                if (sonar.getDistanceCM() <= D2 && !stateTimer.isActive()) {
                    stateTimer.start();
                } else if (sonar.getDistanceCM() > D2) {
                    stateTimer.reset();
                } else if (stateTimer.isExpired(T2)) {
                    door.close();
                    previousState = INSIDE;
                }
            } else if (previousState == TAKEOFF) {
                if (sonar.getDistanceCM() >= D1 && !stateTimer.isActive()) {
                    stateTimer.start();
                } else if (sonar.getDistanceCM() < D1) {
                    stateTimer.reset();
                } else if (stateTimer.isExpired(T1)) {
                    door.close();
                    previousState = DRONEOUT;
                }
            }

            // Uscita o conferma Allarme
            if (currentTemp < TEMP1) {
                currentState = previousState;
            } else if (tempTimer.isExpired(T4)) {
                if (previousState == LANDING) {
                    previousState = INSIDE;
                } else if (previousState == TAKEOFF) {
                    previousState = DRONEOUT;
                }

                door.close();
                digitalWrite(LED_PIN3, HIGH);
                digitalWrite(LED_PIN1, LOW);
                digitalWrite(LED_PIN2, LOW);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("ALARM");
                Serial.println("ALARM:ON");
                currentState = ALARM;
            }
            break;

        case ALARM:
            if (digitalRead(BUTTON_PIN) == HIGH) {
                digitalWrite(LED_PIN3, LOW);
                Serial.println("ALARM:OFF");
                tempTimer.reset();
                
                if (previousState == INSIDE) {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    digitalWrite(LED_PIN1, HIGH);
                    lcd.print("DRONE INSIDE");
                    Serial.println("INSIDE");
                } else {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("DRONE OUT");
                    Serial.println("DRONEOUT");
                }
                currentState = previousState;
            }
            break;
    }
}