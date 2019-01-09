#include <HX711_ADC.h>

/**
 * Port register manipulation
 * https://www.arduino.cc/en/Hacking/Atmega168Hardware
 * 
 * Bank D - D7-D0
 *      B - D13-D8
 *      C - A5-A0
 * 
 * DDRx     - IO direction (0 = input, 1 = output)
 * PORTx    - output (0 = LOW, 1 = HIGH)
 * PINx     - input (0 = LOW, 1 = HIGH)
*/

//  HX711 pins
#define PIN_DOUT    9
#define PIN_CLK     10

//  MKS pins
#define PIN_DIR     A1
#define PIN_TARE    8

//  Probe PCB
#define PIN_TRIGGER 7

//  Calibration constants
#define LOWER_THRESHOLD     20
#define UPPER_THRESHOLD     35
#define SETTLE_TIME         500

//  Comment out to remove debug functionality
#define DEBUG 1

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell(PIN_DOUT, PIN_CLK);

float mCellOutput = 0 ;
bool mFlagReset = false;
bool mDirZ, mDirZLast;


void reset() {
    // digitalWrite(LED_PIN,HIGH);
    digitalWrite(PIN_TRIGGER, LOW);
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
    if (digitalRead(PIN_DIR))
        return true;    //  Moving towards bed
    else
        return false;   //  moving away from bed
}

//  function to check if bed changed direction towards nozzle
bool isBedClosingIn() {
    boolean temp;
    mDirZ = digitalRead(PIN_DIR);
    temp = (mDirZ != mDirZLast && mDirZ == HIGH);
    mDirZLast = mDirZ;
    return temp;
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

    pinMode(PIN_TRIGGER, OUTPUT);
    digitalWrite(PIN_TRIGGER, LOW);
    //pinMode(PIN_TARE,INPUT_PULLUP);
    pinMode(PIN_DIR, INPUT);
    mDirZLast = digitalRead(PIN_DIR);
}

void loop() {
    if (isBedClosingIn())
        reset();

    if (zDirection()) {
        if (isBedClosingIn())
            reset();

        #ifdef DEBUG
            Serial.print(20);
            Serial.print(" ");
        #endif

        LoadCell.update();
        Serial.print(LoadCell.getData());

        mCellOutput = LoadCell.getData();


        if (mCellOutput >= LOWER_THRESHOLD && mCellOutput < UPPER_THRESHOLD) {
            digitalWrite(PIN_TRIGGER, HIGH);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(LOWER_THRESHOLD);
            #endif
        }

        if (mCellOutput >= UPPER_THRESHOLD) {
            digitalWrite(PIN_TRIGGER, HIGH);
            delay (50);
            digitalWrite(PIN_TRIGGER, LOW);
            delay(50);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(LOWER_THRESHOLD);
            #endif
        } else {
            digitalWrite(PIN_TRIGGER, LOW);
            #ifdef DEBUG
                Serial.print(" ");
                Serial.print(0);
            #endif
        }
        #ifdef DEBUG
            Serial.println();
        #endif
    } else {
        digitalWrite(PIN_TRIGGER, LOW);
    }
}
