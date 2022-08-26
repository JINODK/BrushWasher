#include <Arduino.h>
#include "ota.h"
#include "oledHandler.h"

OTA ota;
OLED oled;

/*****HARDWARE PINS DEFINITION*****/
#define PUMP 2
#define VALVE 14
#define HEATER 12
#define FAN 13
#define OK_BUTTON 0
#define UP_BUTTON 16
#define DOWN_BUTTON 15

/*****DEFAULT TIMER CONFIGURATION*****/
uint8_t pumpTime = 3;
uint8_t washTime = 30;
uint8_t drainTime = 10;
uint8_t spinTime = 20;
uint8_t spinSpeed = 100;
uint8_t dryBlow = 200;
uint8_t stage = 0;


void setup() {
    Serial.begin(115200);
    ota.setup("JINODK", "jinodk2003", "Mini-washing-machine");
    oled.setup(); // head to the file to change display's settings
    pinMode(PUMP, OUTPUT);
    pinMode(VALVE, OUTPUT);
    pinMode(HEATER, OUTPUT);
    pinMode(FAN, OUTPUT);
    pinMode(OK_BUTTON, INPUT_PULLUP);
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DOWN_BUTTON, INPUT_PULLUP);
}

void loop() {
    ota.handle();
}