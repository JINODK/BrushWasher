#include <Arduino.h>
#include "ota.h"
#include "oledHandler.h"

OTA ota;
OLED oled;

/*****HARD-CODED CONFIGURATION*****/
char *ssid = "JINODK";
char *password = "jinodk2003";
char *hostname = "Mini-washing-machine";

/*****HARDWARE PINS DEFINITION*****/
#define PUMP 2
#define VALVE 14
#define HEATER 12
#define FAN 13
#define OK_BUTTON 0
#define UP_BUTTON 16
#define DOWN_BUTTON 15

/*****DEFAULT TIMER CONFIGURATION*****/
// uint8_t washTime = 30;
uint8_t drainTime = 10;
uint8_t pumpTime = 3;
uint8_t spinTime = 30;
uint8_t spinSpeed = 100;
uint8_t dryBlow = 200;
uint8_t stage = 0;

int bttnStat = 0; // 1 - up, 2 - down, 3 - ok
char temp[2];
/*****STAGE COUNT
 * 0 - idle - user pour washing additive
 * 1 - spin - washtime
 * 2 - drain
 * 3 - pump clean water
 * 4 - spin - spintime
 * 5 - drain
 * 6 - spin + dryblow
 * 7 - idle
*****/


void ICACHE_RAM_ATTR upBttn() {
    bttnStat = 1;
}

void ICACHE_RAM_ATTR downBttn() {
    bttnStat = 2;
}

void ICACHE_RAM_ATTR okBttn() {
    bttnStat = 3;
}


void setup() {
    Serial.begin(115200);
    ota.setup(ssid, password, hostname);
    oled.setup(); // head to the file to change display's settings
    pinMode(PUMP, OUTPUT);
    pinMode(VALVE, OUTPUT);
    pinMode(HEATER, OUTPUT);
    pinMode(FAN, OUTPUT);
    pinMode(OK_BUTTON, INPUT_PULLUP);
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DOWN_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(UP_BUTTON), upBttn, FALLING);
    attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), downBttn, FALLING);
    attachInterrupt(digitalPinToInterrupt(OK_BUTTON), okBttn, FALLING);
    oled.clear();

}

void loop() {
    switch (stage) {
    case 0:
        oled.write("Set spin time", 0, 0, 1, 0, 0);
        sprintf(temp, "%d", spinTime);
        oled.write(temp, 0, 1, 1, 0, 1);
        while (bttnStat != 3) {
            if (bttnStat == 1) {
                spinTime++;
                if (spinTime > 60) {
                    spinTime = 60;
                }
                sprintf(temp, "%02d", spinTime);
                oled.write(temp, 0, 1, 1, 0, 1);
                bttnStat = 0;
            }
            if (bttnStat == 2) {
                spinTime--;
                if (spinTime < 1) {
                    spinTime = 1;
                }
                sprintf(temp, "%02d", spinTime);
                oled.write(temp, 0, 1, 1, 0, 1);
                bttnStat = 0;
            }
            delay(50);
        }
        bttnStat = 0;
        sprintf(temp, "Spn time: %d", spinTime);
        oled.write(temp, 0, 0, 1, 0, 0);

        oled.write("Set spin speed", 0, 1, 1, 0, 0);
        sprintf(temp, "%d", spinSpeed);
        oled.write(temp, 0, 2, 1, 0, 1);
        while (bttnStat != 3) {
            if (bttnStat == 1) {
                spinSpeed++;
                if (spinSpeed > 60) {
                    spinSpeed = 60;
                }
                sprintf(temp, "%02d", spinSpeed);
                oled.write(temp, 0, 2, 1, 0, 1);
                bttnStat = 0;
            }
            if (bttnStat == 2) {
                spinSpeed--;
                if (spinSpeed < 1) {
                    spinSpeed = 1;
                }
                sprintf(temp, "%02d", spinSpeed);
                oled.write(temp, 0, 2, 1, 0, 1);
                bttnStat = 0;
            }
            delay(50);
        }
        bttnStat = 0;
        sprintf(temp, "Spn speed: %d", spinSpeed);
        oled.write(temp, 0, 1, 1, 0, 0);

        oled.write("Set dry time", 0, 2, 1, 0, 0);
        sprintf(temp, "%d", dryBlow);
        oled.write(temp, 0, 3, 1, 0, 1);
        while (bttnStat != 3) {
            if (bttnStat == 1) {
                dryBlow++;
                if (dryBlow > 60) {
                    dryBlow = 60;
                }
                sprintf(temp, "%02d", dryBlow);
                oled.write(temp, 0, 3, 1, 0, 1);
                bttnStat = 0;
            }
            if (bttnStat == 2) {
                dryBlow--;
                if (dryBlow < 1) {
                    dryBlow = 1;
                }
                sprintf(temp, "%02d", dryBlow);
                oled.write(temp, 0, 3, 1, 0, 1);
                bttnStat = 0;
            }
            delay(50);
        }
        bttnStat = 0;
        sprintf(temp, "Dry time: %d", dryBlow);
        oled.write(temp, 0, 2, 1, 0, 0);
        oled.write("        ", 0, 3, 1, 0, 1);
        delay(1000);
        oled.clear();
        stage++;
        break;
    
    default:
        break;
    }
    ota.handle();
}