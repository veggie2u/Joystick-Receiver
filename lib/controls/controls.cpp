#include <Arduino.h>
#include <utils.h>
#include <debug.h>

#include "controls.h"

Packet theData;  // data we read
Packet checkData;  // data we check for change
Packet_Packed packedData; // data we send

void initControls() {
  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
}

// We want to send the data as small as possible, so the booleans are packed
void packData() {
  packedData.joy_x = theData.joy_x;
  packedData.joy_y = theData.joy_y;
  packedData.buttons = 0;
  if (theData.joy_button) BB_TRUE(packedData.buttons, BB_BUTTON_JOY);
  if (theData.green_button) BB_TRUE(packedData.buttons, BB_BUTTON_GREEN);
  if (theData.blue_button) BB_TRUE(packedData.buttons, BB_BUTTON_BLUE);
}

void unpackData() {
  theData.joy_x = packedData.joy_x;
  theData.joy_y = packedData.joy_y;

  theData.joy_button = BB_READ(packedData.buttons, BB_BUTTON_JOY);
  theData.green_button = BB_READ(packedData.buttons, BB_BUTTON_GREEN);
  theData.blue_button = BB_READ(packedData.buttons, BB_BUTTON_BLUE);
}

// reads an analog joystick axis
int getAxis(unsigned char axis) {
  int joy = analogRead(axis);
  joy = map(joy, 0, 1023, -255, 255);
  if( abs(joy) < 10 ) {
    joy = 0;
  }
  return joy;  
}

// reads the inputs and puts them in theData struct
void readJoystick() {
  theData.joy_x = getAxis(XAXIS);
  theData.joy_y = getAxis(YAXIS);
  theData.joy_button = digitalRead(JOY_BUTTON_PIN) == LOW;
  theData.green_button = digitalRead(GREEN_BUTTON_PIN) == LOW;
  theData.blue_button = digitalRead(BLUE_BUTTON_PIN) == LOW;
}

// return true if any joystick data changes
boolean hasJoystickChanged() {
  return !(
    theData.joy_x == checkData.joy_x &&
    theData.joy_y == checkData.joy_y &&
    theData.joy_button == checkData.joy_button &&
    theData.green_button == checkData.green_button &&
    theData.blue_button == checkData.blue_button);
}

// copy our last send data to our check data
void copyJoystickData() {
  checkData = theData;
}

// print the data we are sending
void printJoystick() {
  debugD("Values... ");
  debugD("joy_x: ");
  debugD(theData.joy_x);
  debugD(" joy_y: ");
  debugD(theData.joy_y);
  debugD(" joy_buton: ");
  debugD(theData.joy_button);
  debugD(" green_button: ");
  debugD(theData.green_button);
  debugD(" blue_button: ");
  debuglnD(theData.blue_button);
}

Packet_Packed getPackedData() {
    return packedData;
}

Packet getData() {
  return theData;
}

void setPackedData(Packet_Packed data) {
  packedData = data;
}