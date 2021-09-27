#include <Arduino.h>
#include <utils.h>
#include <debug.h>
#include <data.h>
#include "controls.h"

Packet checkData;  // data we check for change

void initControls() {
  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
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
Packet readJoystick() {
  Packet data;
  data.joy_x = getAxis(XAXIS);
  data.joy_y = getAxis(YAXIS);
  data.joy_button = digitalRead(JOY_BUTTON_PIN) == LOW;
  data.green_button = digitalRead(GREEN_BUTTON_PIN) == LOW;
  data.blue_button = digitalRead(BLUE_BUTTON_PIN) == LOW;
  return data;
}

// return true if any joystick data changes
boolean hasJoystickChanged(Packet data) {
  return !(
    data.joy_x == checkData.joy_x &&
    data.joy_y == checkData.joy_y &&
    data.joy_button == checkData.joy_button &&
    data.green_button == checkData.green_button &&
    data.blue_button == checkData.blue_button);
}

// copy our last send data to our check data
void copyJoystickData(Packet data) {
  checkData = data;
}

// print the data we are sending
void printJoystick(Packet data) {
  debugD("joy_x: ");
  debugD(data.joy_x);
  debugD(" joy_y: ");
  debugD(data.joy_y);
  debugD(" joy_buton: ");
  debugD(data.joy_button);
  debugD(" green_button: ");
  debugD(data.green_button);
  debugD(" blue_button: ");
  debuglnD(data.blue_button);
}
