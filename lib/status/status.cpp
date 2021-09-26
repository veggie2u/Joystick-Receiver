
#include <Adafruit_NeoPixel.h>

#include "status.h"

Adafruit_NeoPixel pixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void trafficOn() {
  digitalWrite(LED, HIGH);
}

void trafficOff() {
  digitalWrite(LED, LOW);
}

void initStatus() {
  pinMode(LED, OUTPUT);
  pixel.begin();
  pixel.setBrightness(BRIGHTNESS);
  statusStart();
}

void statusStart() {
  pixel.setPixelColor(0, pixel.Color(0, 0, 150));
  pixel.show();
}

void statusOk() {
  pixel.setPixelColor(0, pixel.Color(0, 150, 0));
  pixel.show();
}

void statusError() {
  pixel.setPixelColor(0, pixel.Color(150, 0, 0));
  pixel.show();
}

