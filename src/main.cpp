#include "configurations.h"
#ifdef OTA
#include "ota.h"
#endif
#include "oledHandler.h"
#include <Servo.h>

#ifdef OTA
OTA ota;
#endif
OLED oled;
Servo servo;

int bttnStat = 0; // 1 - up, 2 - down, 3 - ok
int hold = 0;
unsigned long currTime, targetTime;
char temp[32];
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

#ifdef USE_BUTTON_INTERRUPT
void IRAM_ATTR upBttn() {
    bttnStat = 1;
}

void IRAM_ATTR downBttn() {
    bttnStat = 2;
}

void IRAM_ATTR okBttn() {
    bttnStat = 3;
}
#endif

void taskTimer(char* taskTitle, uint8_t taskTime) {
    unsigned long tarTime = millis() + taskTime * 1000;
    oled.clear();
    oled.write(taskTitle, 0, 0, 1, 0, 1);
    while (millis() < tarTime) {
        sprintf(temp, "%02lu sec left", (tarTime - millis()) / 1000);
        oled.write(temp, 0, 7, 1, 0, 1);
        if (!digitalRead(OK_BUTTON)) { // cancel function
            oled.write((char *)"Cancelling in", 0, 1, 1, 0, 1);
            sprintf(temp, "%d / 50", hold);
            oled.write(temp, 0, 2, 1, 0, 1);
            // bttnStat = 0;
            if (hold == 50) {
                // disable everything
                servo.write(0);
                digitalWrite(PUMP, LOW);
                digitalWrite(VALVE, LOW);
                digitalWrite(HEATER, LOW);
                digitalWrite(FAN, LOW);

                oled.clear();
                oled.write((char *)"User", 0, 0, 2, 0, 1);
                oled.write((char *)"aborted", 0, 3, 2, 0, 1);
                delay(1000);
                bttnStat = 0;
                oled.write((char *)"press to restart", 0, 7, 1, 0, 1);
                #ifdef USE_BUTTON_INTERRUPT
                while (bttnStat != 3) {
                #else
                while (!digitalRead(OK_BUTTON)) {
                #endif
                    #ifdef OTA
                    ota.handle();
                    #endif
                    delay(1);
                }
                ESP.restart();

            } else {
                hold++;
            }
            #ifdef OTA
            ota.handle();
            #endif
        } else if (hold) {
            oled.write((char *)"            ", 0, 1, 1, 0, 1);
            oled.write((char *)"            ", 0, 2, 1, 0, 1);
            hold = 0;
        }
        #ifdef OTA
        ota.handle();
        #endif
        delay(1);
    }
    oled.clear();
    sprintf(temp, "%s done", taskTitle);
    oled.write(temp, 0, 0, 1, 0, 1);
}

void setup() { 
    /*****SETUP*****/
    Serial.begin(115200);
    #ifdef OTA
    ota.setup(ssid, password, hostname);
    #endif
    oled.setup(); // head to the file to change display's settings
    pinMode(PUMP, OUTPUT);
    pinMode(VALVE, OUTPUT);
    pinMode(HEATER, OUTPUT);
    pinMode(FAN, OUTPUT);
    pinMode(OK_BUTTON, INPUT_PULLUP);
    // pinMode(UP_BUTTON, INPUT_PULLUP);
    // pinMode(DOWN_BUTTON, INPUT_PULLUP);
    servo.attach(MOTOR);

    // arm the ESC
    servo.writeMicroseconds(1600);
    delay(10);
    servo.writeMicroseconds(1000);
    // turn everything off just to be safe
    digitalWrite(PUMP, LOW);
    digitalWrite(VALVE, LOW);
    digitalWrite(HEATER, LOW);
    digitalWrite(FAN, LOW);

    #ifdef USE_BUTTON_INTERRUPT
    // button interrupt detection
    // attachInterrupt(digitalPinToInterrupt(UP_BUTTON), upBttn, FALLING);
    // attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), downBttn, FALLING);
    attachInterrupt(digitalPinToInterrupt(OK_BUTTON), okBttn, FALLING);
    #endif
    oled.clear();


    /*****ACTUAL PROGRAM*****/
    oled.write((char *)"Press button to", 0, 0, 1, 0, 1);
    oled.write((char *)"start...", 0, 1, 1, 0, 1);
    #ifdef USE_BUTTON_INTERRUPT
    while (bttnStat != 3) {
    #else
    while (digitalRead(OK_BUTTON)) { // no ! because of waiting for button to be pressed
    #endif
        #ifdef OTA
        ota.handle();
        #endif
        delay(1);
    }
    oled.clear();
    delay(100);
    // spin motor
    servo.write(spinSpeed);
    taskTimer((char *)"Washing", spinTime);
    // stop motor
    servo.write(0);

    delay(1000);
    // drain water
    digitalWrite(VALVE, HIGH);
    taskTimer((char *)"Drain", drainTime);
    // close valve
    digitalWrite(VALVE, LOW);

    delay(1000);
    // start pump
    digitalWrite(PUMP, HIGH);
    taskTimer((char *)"Pump", pumpTime);
    // stop pump
    digitalWrite(PUMP, LOW);

    delay(1000);
    // spin motor
    servo.write(spinSpeed);
    taskTimer((char *)"Spin", spinTime);
    // stop motor
    servo.write(0);

    delay(1000);
    // drain water
    digitalWrite(VALVE, HIGH);
    taskTimer((char *)"Drain", drainTime);
    // close valve
    digitalWrite(VALVE, LOW);

    delay(1000);
    // blow dry
    digitalWrite(FAN, HIGH);
    digitalWrite(HEATER, HIGH);
    servo.write(spinSpeed);
    taskTimer((char *)"Dry blow", dryBlow);
    // stop dryer
    digitalWrite(FAN, LOW);
    digitalWrite(HEATER, LOW);
    servo.write(0);

    oled.clear();
    oled.write((char *)"Task done! Press", 0, 0, 1, 0, 1);
    oled.write((char *)"button to", 0, 1, 1, 0, 1);
    oled.write((char *)"restart", 0, 2, 1, 1, 1);
    delay(100);
    bttnStat = 0;
    #ifdef USE_BUTTON_INTERRUPT
    while (bttnStat != 3) {
    #else
    while (digitalRead(OK_BUTTON)) { // no ! because of waiting for button to be pressed
    #endif
        #ifdef OTA
        ota.handle();
        #endif
        delay(1);
    }
    oled.clear();
    oled.write((char *)"         owo", 0, 7, 0, 0, 1);
    oled.write((char *)"            ", 0, 7, 0, 0, 1);
    ESP.restart();
}

void loop() {
    #ifdef OTA // wait how did you get here
    ota.handle();
    #endif
    delay(1);
    // switch (stage) {
    // case 0: // setup
    //     oled.write((char *)"Set spin time", 0, 0, 1, 0, 0);
    //     sprintf(temp, "%d", spinTime);
    //     oled.write(temp, 0, 1, 1, 0, 1);
    //     while (bttnStat != 3) {
    //         if (bttnStat == 1) {
    //             spinTime++;
    //             if (spinTime > 60) {
    //                 spinTime = 60;
    //             }
    //             sprintf(temp, "%02d", spinTime);
    //             oled.write(temp, 0, 1, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         if (bttnStat == 2) {
    //             spinTime--;
    //             if (spinTime < 1) {
    //                 spinTime = 1;
    //             }
    //             sprintf(temp, "%02d", spinTime);
    //             oled.write(temp, 0, 1, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         delay(50);
    //     }
    //     bttnStat = 0;
    //     sprintf(temp, "Spn time: %d", spinTime);
    //     oled.write(temp, 0, 0, 1, 0, 0);

    //     oled.write((char *)"Set spin speed", 0, 1, 1, 0, 0);
    //     sprintf(temp, "%d", spinSpeed);
    //     oled.write(temp, 0, 2, 1, 0, 1);
    //     while (bttnStat != 3) {
    //         if (bttnStat == 1) {
    //             spinSpeed++;
    //             if (spinSpeed > 60) {
    //                 spinSpeed = 60;
    //             }
    //             sprintf(temp, "%02d", spinSpeed);
    //             oled.write(temp, 0, 2, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         if (bttnStat == 2) {
    //             spinSpeed--;
    //             if (spinSpeed < 1) {
    //                 spinSpeed = 1;
    //             }
    //             sprintf(temp, "%02d", spinSpeed);
    //             oled.write(temp, 0, 2, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         delay(50);
    //     }
    //     bttnStat = 0;
    //     sprintf(temp, "Spn speed: %d", spinSpeed);
    //     oled.write(temp, 0, 1, 1, 0, 0);

    //     oled.write((char *)"Set dry time", 0, 2, 1, 0, 0);
    //     sprintf(temp, "%d", dryBlow);
    //     oled.write(temp, 0, 3, 1, 0, 1);
    //     while (bttnStat != 3) {
    //         if (bttnStat == 1) {
    //             dryBlow++;
    //             if (dryBlow > 60) {
    //                 dryBlow = 60;
    //             }
    //             sprintf(temp, "%02d", dryBlow);
    //             oled.write(temp, 0, 3, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         if (bttnStat == 2) {
    //             dryBlow--;
    //             if (dryBlow < 1) {
    //                 dryBlow = 1;
    //             }
    //             sprintf(temp, "%02d", dryBlow);
    //             oled.write(temp, 0, 3, 1, 0, 1);
    //             bttnStat = 0;
    //         }
    //         delay(50);
    //     }
    //     bttnStat = 0;
    //     sprintf(temp, "Dry time: %d", dryBlow);
    //     oled.write(temp, 0, 2, 1, 0, 0);
    //     oled.write((char *)"        ", 0, 3, 1, 0, 1);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;
    
    // case 1: // washing
    //     oled.write((char *)"Washing", 0, 0, 1, 0, 0);
    //     currTime = millis();
    //     targetTime = currTime + (spinTime * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (spinTime - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Washing done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;
    
    // case 2: // draining
    //     oled.write((char *)"Draining", 0, 0, 1, 0, 0);
    //     // digitalWrite(VALVE, HIGH);
    //     currTime = millis();
    //     targetTime = currTime + (drainTime * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (drainTime - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Draining done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;
    
    // case 3: // pumping clean water
    //     oled.write((char *)"Pumping", 0, 0, 1, 0, 0);
    //     currTime = millis();
    //     targetTime = currTime + (pumpTime * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (pumpTime - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Pumping done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;
    
    // case 4: // spinning
    //     oled.write((char *)"Spin", 0, 0, 1, 0, 0);
    //     currTime = millis();
    //     targetTime = currTime + (spinTime * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (spinTime - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Spin done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;
    // case 5: // draining
    //     oled.write((char *)"Draining", 0, 0, 1, 0, 0);
    //     // digitalWrite(VALVE, HIGH);
    //     currTime = millis();
    //     targetTime = currTime + (drainTime * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (drainTime - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Draining done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;

    // case 6: // dry blow
    //     oled.write((char *)"Dry blow", 0, 0, 1, 0, 0);
    //     // digitalWrite(VALVE, HIGH);
    //     currTime = millis();
    //     targetTime = currTime + (dryBlow * 1000);
    //     while (millis() < targetTime) {
    //         // TODO: add cancel function: hold ok button for 1 seconds
    //         if (millis() - currTime > 1000) {
    //             sprintf(temp, "%02lu secs left", (dryBlow - (millis() - currTime) / 1000));
    //             oled.write(temp, 0, 7, 1, 0, 1);
    //             currTime = millis();
    //         }
    //     }
    //     oled.clear();
    //     oled.write((char *)"Drying done", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     stage++;
    //     break;

    // case 7: // done
    //     oled.write((char *)"Done", 0, 0, 1, 0, 0);
    //     while (bttnStat != 3);
    //     oled.write((char *)"Halting...", 0, 0, 1, 0, 0);
    //     delay(1000);
    //     oled.clear();
    //     for(;;); //halt
    //     break;

    // default:
    //     break;
    // }
    // ota.handle();
}