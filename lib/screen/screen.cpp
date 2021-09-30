#include "screen.h"

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#if SCREEN_TYPE == 1107
#include <Adafruit_SH110X.h>
#else
#include <Adafruit_SSD1306.h>
#endif
#include <Wire.h>

#include <status.h>
#include <controls.h>

#if SCREEN_TYPE == 1107 
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
#else
Adafruit_SSD1306 display = Adafruit_SSD1306();
#endif



void initOled(const char radioType[]) {
    #if SCREEN_TYPE == 1107
    display.begin(SCREEN_ADDRESS, true);
    display.setTextColor(SH110X_WHITE);
    display.setRotation(1);
    #else
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x32)
    display.setTextColor(WHITE);
    #endif
    display.setTextSize(1);
    display.display(); // start up picture
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever "); display.print(radioType); display.print(" start");
    display.display();
}

void printToOled(char* status, float batVoltage, int16_t rssi, Packet theData) {
    char charBuf[40];    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever:"); display.print(status);
    sprintf(charBuf, "VBat:%.2fv RSSI:%i", batVoltage, rssi);
    display.setCursor(0,8);
    display.print(charBuf);
    display.setCursor(0,16);
    display.print("X:");
    display.print(theData.joy_x);
    display.setCursor(36,16);
    display.print(" Y:");
    display.print(theData.joy_y);
    display.setCursor(0,24);
    display.print("Buttons :");
    display.print(theData.joy_button);
    display.print(" ");
    display.print(theData.green_button);
    display.print(" ");
    display.print(theData.blue_button);
    display.display();
}

void printErrorOled(char* status) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever Error:");
    display.setCursor(0,16);
    display.print(status);
    display.display();
}