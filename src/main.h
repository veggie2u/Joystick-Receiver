#include <Arduino.h>

#define NETWORKID     100
//#define FREQUENCY     RF69_915MHZ
#define RF69_FREQ     915.0
#define ENCRYPTKEY    "cyberwardrobot00"
#define IS_RFM69HCW   true

enum Controls {
  redButton,
  greenButton,
  yellowButton,
  blueButton
};

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

#define BB_BUTTON_JOY     1
#define BB_BUTTON_GREEN   2
#define BB_BUTTON_BLUE    4

//************************************************************************
//****** Real Bit Level Boolean Utilisation Macro Definitions  ***********
//****** Created by Trevor D.BEYDAG 09 May 2016                ***********
//****** http://lab.dejaworks.com                              ***********
//****** Please keep credits intact when using and/or reproducing ********
//************************************************************************
#define BB_TRUE(bp,bb)    bp |= bb
#define BB_FALSE(bp,bb)   bp &= ~(bb)
#define BB_READ(bp,bb)    bool(bp & bb)
