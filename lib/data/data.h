#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

#define BB_BUTTON_JOY     1
#define BB_BUTTON_GREEN   2
#define BB_BUTTON_BLUE    4

typedef struct {
  int    joy_x;
  int    joy_y;
  bool   joy_button;
  bool   green_button;
  bool   blue_button;
} Packet;

typedef struct {
  int joy_x;
  int joy_y;
  uint8_t buttons;
} Packet_Packed;

void packData();
void unpackData();
Packet_Packed getPackedData();
Packet getData();
void setPackedData(Packet_Packed data);
void setData(Packet data);

#endif