#ifndef CONTROLS_H 
#define CONTROLS_H 

#include <data.h>

#define XAXIS             A2
#define YAXIS             A3
#define JOY_BUTTON_PIN    12
#define GREEN_BUTTON_PIN  A5
#define BLUE_BUTTON_PIN   A4

void initControls();
int getAxis();
Packet readJoystick();
boolean hasJoystickChanged(Packet data);
void copyJoystickData(Packet data);
void printJoystick(Packet data);

#endif