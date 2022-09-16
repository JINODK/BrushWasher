#pragma once
#include <Arduino.h>

/*****FLAGS*****/
// #define OTA
// #define USE_BUTTON_INTERRUPT
#define DEBUG

/*****HARD-CODED CONFIGURATION*****/
const char* ssid = "JINODK";
const char* password = "jinodk2003";
const char* hostname = "Mini-washing-machine";

/*****HARDWARE PINS DEFINITION*****/
#define PUMP_IN 15     // D8
#define PUMP_OUT 14    // D5
#define HEATER 12   // D6
#define FAN 13      // D7
#define OK_BUTTON 0 // D3
// #define UP_BUTTON 15 // D8
#define MOTOR 16    // D0

/*****DEFAULT TIMER CONFIGURATION*****/
// uint8_t washTime = 30;
uint8_t spinTime = 30;//30;
uint8_t spinSpeed = 16; // out of 90
uint8_t spinSpeed2 = 26; // drying spin speed
uint8_t cleanPump = 3; // pump water to clean bubbles
uint8_t drainTime = 7;//10;
uint8_t cleanFlush = 5; // flush bubble water
uint8_t pumpTime = 7;
uint8_t dryBlow = 45;//200;
// uint8_t stage = 1; // skip over setup stage