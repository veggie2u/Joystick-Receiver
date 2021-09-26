#ifndef CONTROLS_H 
#define CONTROLS_H 

#define XAXIS             A2
#define YAXIS             A3
#define JOY_BUTTON_PIN    12
#define GREEN_BUTTON_PIN  A5
#define BLUE_BUTTON_PIN   A4

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

void initControls();
void packData();
void unpackData();
int getAxis();
void readJoystick();
boolean hasJoystickChanged();
void copyJoystickData();
void printJoystick();
Packet_Packed getPackedData();
Packet getData();
void setPackedData(Packet_Packed data);
#endif