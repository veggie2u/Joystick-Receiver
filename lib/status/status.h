// definitions and functions to handle the status led

#ifndef STATUS_H 
#define STATUS_H 

#define NUMPIXELS       1 // onboard pixel
#define DATAPIN         8
#define CLOCKPIN        6
#define BRIGHTNESS      100
#define NEOPIXEL_PIN    8

#define LED             13  // onboard blinky

void trafficOn();
void trafficOff();

void initStatus();
void statusStart();
void statusOk();
void statusError();

#endif
