#include "data.h"
#include <utils.h>

Packet theData;  // data we read
Packet_Packed packedData; // data we send

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

Packet_Packed getPackedData() {
    return packedData;
}

Packet getData() {
  return theData;
}

void setPackedData(Packet_Packed data) {
  packedData = data;
}

void setData(Packet data) {
    theData = data;
}