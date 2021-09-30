#ifndef SCREEN_H 
#define SCREEN_H 

#include <Arduino.h>
#include <controls.h>

#define SCREEN_TYPE     1107 // SH1107 or SSD1306
#define SCREEN_WIDTH    128  // OLED display width, in pixels
#define SCREEN_HEIGHT   64   // OLED display height, in pixels
#define OLED_RESET      -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS  0x3C 

void initOled(const char radioType[]);
void printToOled(char* status, float batVoltage, int16_t rssi, Packet theData);
void printErrorOled(char* status);

#endif