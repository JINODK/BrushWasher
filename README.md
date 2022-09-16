# BrushWasher
Esp8266-based brush washer with multiple cleaning modes, support for OLED SSD1306 screen and OTA update
# Setup
This project can be imported directly into `PlatformIO` to automatically download the required library and compile the code
## Library
- ss_oled: https://github.com/bitbank2/ss_oled
## Wiring
Wiring can be changed in `configuration.h`, except for diplay wiring, which could be changed in `oledHandler.h`
```
#define PUMP_IN 15     // D8
#define PUMP_OUT 14    // D5
#define HEATER 12   // D6
#define FAN 13      // D7
#define OK_BUTTON 0 // D3
// #define UP_BUTTON 15 // D8
#define MOTOR 16    // D0
```
|Pin function|ESP8266 GPIO|NodeMCU Pin|
|------------|------------|-----------|
|PUMP_IN     |15          |D8         |
|PUMP_OUT    |14          |D5         |
|Heater      |12          |D6         |
|FAN         |13          |D7         |
|OK_BUTTON   |0           |D3         |
|MOTOR       |16          |D0         |
|SDA         |4           |D2         |
|SCL         |5           |D1         |
