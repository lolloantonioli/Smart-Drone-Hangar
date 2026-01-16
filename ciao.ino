#include <Servo.h>

#include <LiquidCrystal_I2C.h>

#define LED_PIN1 2
#define LED_PIN2 3
#define LED_PIN3 4
#define BUTTON_PIN 6

#define TEMP1 30

#define T1 2000
#define T2 2000
#define T3 3000
#define T4 5000
#define D1 3.00
#define D2 1.50
#define TEMP1 30

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); 

Servo doorServo;
long timer = 0;
const int DOOR_CLOSED = 0;
const int DOOR_OPEN = 90;
const int trigPin = 13;
const int echoPin = 12;
//supponendo temperature 20 gradi
const float vs = 331.5 + 0.6*20;

int inputPin = 11; //choose the input pin
int pirState = LOW; //assuming no motion detected, SERVE???
int val = 0; //variable for reading the pin status

enum State {
    INSIDE,
    STARTING_TAKEOFF,
    TAKEOFF,
    DRONEOUT,
    LANDING,
    PREALARM,
    ALARM
};

State prevState;
State hangarState;
long time;
int value=0;
float value_in_Volt=0;
float value_in_C=0;

float getDistance()
{
 /* sending impulse */
 digitalWrite(trigPin,LOW);
 delayMicroseconds(3);
 digitalWrite(trigPin,HIGH);
 delayMicroseconds(5);
 digitalWrite(trigPin,LOW);
 
 /* receiving the eco */
 float tUS = pulseIn(echoPin, HIGH);
 float t = tUS / 1000.0 / 1000.0 / 2;
 float d = t*vs;
 return d;
}


void setup()
{
 	Serial.begin(9600);
  	/*hangarState=INSIDE;
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);

    lcd.setCursor(0, 0);
    lcd.print("DRONE INSIDE");*/
    hangarState=STARTING_TAKEOFF; 
  	//segnali attuali da sostituire con DRU (GUI-based system)
  
  	//led
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
  	//lcd
  	lcd.init();
    lcd.backlight();
  	//servo init
  	doorServo.attach(5);      
    doorServo.write(DOOR_CLOSED);
  	//sonar
  	pinMode(trigPin, OUTPUT);
 	pinMode(echoPin, INPUT); 
  	//radar
  	pinMode(inputPin, INPUT);  
}

void loop()
{
  switch(hangarState){
  	case INSIDE:
        value = analogRead(A3);
        value_in_Volt = ((float) value)* 0.00488;   //  5 Volt / 1024 range = 0.00488
        // TMP36 characteristics: V = Temp * 0.01 + 0.5
        value_in_C = (value_in_Volt - 0.5) / 0.01;  // for TMP36 T(C°) = Volt - 0.5)
        Serial.println(value_in_C);

        if(value_in_C >= TEMP1 && timer==0) {
          timer=millis();
    
        } else if(value_in_C < TEMP1) {
          timer=0;
        } else if(timer!=0 && millis()-timer > T3){
          doorServo.write(DOOR_CLOSED);
          lcd.setCursor(0, 0);
          prevState=hangarState;
          timer=millis();
          hangarState=PREALARM;
        }
  
      	//aggiungere rilevazione segnale da DRU
    	//hangarState=STARTING_TAKEOFF;
        break;
    
    case STARTING_TAKEOFF:
        value = analogRead(A3);
        value_in_Volt = ((float) value)* 0.00488;   //  5 Volt / 1024 range = 0.00488
        // TMP36 characteristics: V = Temp * 0.01 + 0.5
        value_in_C = (value_in_Volt - 0.5) / 0.01;  // for TMP36 T(C°) = Volt - 0.5)
        Serial.println(value_in_C);
    	digitalWrite(LED_PIN1, LOW);
    	lcd.clear();
    	doorServo.write(DOOR_OPEN);
        lcd.setCursor(0, 0);
        lcd.print("TAKE OFF");
    	time=0;
    	hangarState=TAKEOFF;
    	break;
    
    case TAKEOFF:
        value = analogRead(A3);
        value_in_Volt = ((float) value)* 0.00488;   //  5 Volt / 1024 range = 0.00488
        // TMP36 characteristics: V = Temp * 0.01 + 0.5
        value_in_C = (value_in_Volt - 0.5) / 0.01;  // for TMP36 T(C°) = Volt - 0.5)
        Serial.println(value_in_C);
    	digitalWrite(LED_PIN2, HIGH);
    	delay(500);
    	digitalWrite(LED_PIN2,LOW);
    	delay(500);
        if(getDistance()>=D1 && time==0) {
          time=millis();
    
        } else if(getDistance()<D1) {
          time=0;
        } else if(time!=0 && millis()-time>T1){
          doorServo.write(DOOR_CLOSED);
          lcd.setCursor(0, 0);
          lcd.print("DRONE OUT");
          hangarState=DRONEOUT;
        }
        if(value_in_C >= TEMP1 && timer==0) {
          timer=millis();
    
        } else if(value_in_C < TEMP1) {
          timer=0;
        } else if(timer!=0 && millis()-timer > T3){
          doorServo.write(DOOR_CLOSED);
          lcd.setCursor(0, 0);
          prevState=hangarState;
          timer=millis();
          hangarState=PREALARM;
        }
    	break;
    
    case DRONEOUT:
    	//segnale DRU per ritorno in base
    	val = digitalRead(inputPin); 
        if (val == HIGH) {      
          lcd.clear();
          doorServo.write(DOOR_OPEN);
          lcd.setCursor(0, 0);
          lcd.print("LANDING");
          time=0;
          hangarState=LANDING;
          /*if (pirState == LOW) {
            pirState = HIGH;
          } NON CI SERVE(?)*/
        }
    	break;
    
    case LANDING:
        value = analogRead(A3);
        value_in_Volt = ((float) value)* 0.00488;   //  5 Volt / 1024 range = 0.00488
        // TMP36 characteristics: V = Temp * 0.01 + 0.5
        value_in_C = (value_in_Volt - 0.5) / 0.01;  // for TMP36 T(C°) = Volt - 0.5)
        Serial.println(value_in_C);
    	digitalWrite(LED_PIN2, HIGH);
    	delay(500);
    	digitalWrite(LED_PIN2,LOW);
    	delay(500);
    	if(getDistance()<=D2 && time==0) {
          time=millis();
    
        } else if(getDistance()>D2) {
          time=0;
        } else if(time!=0 && millis()-time>T2){
          doorServo.write(DOOR_CLOSED);
          lcd.setCursor(0, 0);
          lcd.print("DRONE INSIDE");
          digitalWrite(LED_PIN1, HIGH);
          hangarState=INSIDE;
        }
        if(value_in_C >= TEMP1 && timer==0) {
          timer=millis();
    
        } else if(value_in_C < TEMP1) {
          timer=0;
        } else if(timer!=0 && millis()-timer > T3){
          doorServo.write(DOOR_CLOSED);
          lcd.setCursor(0, 0);
          prevState=hangarState;
          timer=millis();
          hangarState=PREALARM;
        }
    	break;

    case PREALARM:
        switch(prevState){
            case LANDING:
                if(getDistance()<=D2 && time==0) {
                    time=millis();
                
                } else if(getDistance()>D2) {
                    time=0;
                } else if(time!=0 && millis()-time>T2){
                    doorServo.write(DOOR_CLOSED);
                    prevState=INSIDE;
                }

                break;
            case TAKEOFF:
                if(getDistance()>=D1 && time==0) {
                    time=millis();               
                } else if(getDistance()<D1) {
                    time=0;
                } else if(time!=0 && millis()-time>T1){
                    doorServo.write(DOOR_CLOSED);
                    prevState=DRONEOUT;
                }
                break;
            default:
                break;
        }

        value = analogRead(A3);
        value_in_Volt = ((float) value)* 0.00488;   //  5 Volt / 1024 range = 0.00488
        // TMP36 characteristics: V = Temp * 0.01 + 0.5
        value_in_C = (value_in_Volt - 0.5) / 0.01;  // for TMP36 T(C°) = Volt - 0.5)
        if(value_in_C < TEMP1){
            hangarState=prevState;
        } else if(millis() - timer > T4){
            doorServo.write(DOOR_CLOSED);
            digitalWrite(LED_PIN3,HIGH);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALARM");
            hangarState=ALARM;
        }
        break;
    
    case ALARM:
        if (digitalRead(BUTTON_PIN) == HIGH) {
            digitalWrite(LED_PIN3, LOW);
            if(prevState==INSIDE){
              lcd.clear();
              lcd.setCursor(0, 0);
              digitalWrite(LED_PIN1, HIGH);
              lcd.print("DRONE INSIDE");
            } else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("DRONE OUT");
            }
            hangarState=prevState;
        }
  }
    
  
}