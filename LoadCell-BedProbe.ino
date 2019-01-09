//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library
// Settling time (number of samples) and data filtering can be adjusted in the HX711_ADC.h file

#include <HX711_ADC.h>

#define PIN_DOUT    9
#define PIN_CLK     10

#define PIN_DIR     A1
#define PIN_TARE    8
#define PIN_TRIGGER 7

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell(9, 10);

float val = 0 ;
bool resetFlag = false;
bool dir, prevDir;

#define LOWERTHRESHOLD 20
#define UPPERTHRESHOLD 35
#define DIR_PIN A1
#define TARE_PIN 8
#define TRIGGER_PIN 7
#define SETTLE_TIME 500

#define DEBUG 1


void reset() {
    // digitalWrite(LED_PIN,HIGH);
    digitalWrite(TRIGGER_PIN,LOW);
    delay(SETTLE_TIME);
    LoadCell.begin();
    LoadCell.start(4000);
    LoadCell.setCalFactor(700); // user set calibration factor (float)
    #ifdef DEBUG
        Serial.print(35);
        Serial.print(" ");
    #endif
}

bool zDirection() {
    if (digitalRead(DIR_PIN))
        return true;    //  Moving towards from bed
    else
        return false;   //  moving away from bed
}

//  function to check if bed changed direction towards nozzle
bool aproachingNozzle() {
    dir = digitalRead(DIR_PIN);
    if ((dir == HIGH) && (prevDir == LOW)) {
        prevDir=dir;
        return true;
    }  else{
        prevDir=dir;
        return false;
    }
}

void setup() {
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Wait...");
    #endif

    LoadCell.begin();
    LoadCell.start(2000);
    LoadCell.setCalFactor(700.25); // user set calibration factor (float)

    #ifdef DEBUG
        Serial.println("Startup + tare is complete");
    #endif

    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW);
    //pinMode(TARE_PIN,INPUT_PULLUP);
    pinMode(DIR_PIN, INPUT);
    prevDir = digitalRead(DIR_PIN);
}

void loop(){
    if (aproachingNozzle())
        reset();

    if (zDirection()) {
        if (aproachingNozzle())
            reset();

        #ifdef DEBUG
            Serial.print(20);
            Serial.print(" ");
        #endif

        LoadCell.update();
        Serial.print(LoadCell.getData());

        val = LoadCell.getData();


        if (val >= LOWERTHRESHOLD && val < UPPERTHRESHOLD) {
            digitalWrite(TRIGGER_PIN, HIGH);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(LOWERTHRESHOLD);
            #endif
        }

        if (val >= UPPERTHRESHOLD) {
            digitalWrite(TRIGGER_PIN, HIGH);
            delay (50);
            digitalWrite(TRIGGER_PIN, LOW);
            delay(50);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(LOWERTHRESHOLD);
            #endif
        } else {
            digitalWrite(TRIGGER_PIN, LOW);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(0);
            #endif
        }
        #ifdef DEBUG
            Serial.println();
        #endif
    } else {
        digitalWrite(TRIGGER_PIN, LOW);
    }
}
