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
#define PUMP 15
#define VALVE 14
#define HEATER 12
#define FAN 13
#define OK_BUTTON 0
// #define UP_BUTTON 15
#define MOTOR 16

/*****DEFAULT TIMER CONFIGURATION*****/
// uint8_t washTime = 30;
uint8_t spinTime = 5;//30;
uint8_t spinSpeed = 25; // out of 90
uint8_t drainTime = 5;//10;
uint8_t pumpTime = 3;
uint8_t dryBlow = 6;//200;
// uint8_t stage = 1; // skip over setup stage