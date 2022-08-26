#include <Arduino.h>
#include "ota.h"

OTA ota;
uint8_t pumpTime = 3;
uint8_t washTime = 30;
uint8_t drainTime = 10;
uint8_t spinTime = 20;
uint8_t spinSpeed = 100;
uint8_t dryBlow = 200;
uint8_t stage = 0;



void setup() {
  ota.setup("JINODK", "jinodk2003", "Mini-washing-machine");
}

void loop() {
  ota.handle();
}