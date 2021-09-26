#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <status.h>
#include <controls.h>
#include "screen.h"

Adafruit_SSD1306 display = Adafruit_SSD1306();

void initOled() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    display.setTextSize(1);
    display.setTextColor(WHITE);
    Serial.println("OLED begun");
    display.display(); // start up picture
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever RFM69 start");
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