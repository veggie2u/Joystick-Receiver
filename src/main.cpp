#include <Arduino.h>

/*
  Using RadioHead RFM69 library
*/

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#include "main.h"

#define NODEID        1  
#define TRANSMITTER   2

#define SERIAL_BAUD   115200

/* for Feather radio */
#define RFM69_CS      10
#define RFM69_INT     6
#define RFM69_RST     11

#define NEOPIXEL_PIN  8
#define NUMPIXELS     1

#define LED           13  // onboard blinky
#define DEBUG         true
#define OLED          true
#define VBAT_PIN      A6

int16_t packetnum = 0;  // packet counter, we increment per xmission

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);
// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, NODEID);

Adafruit_SSD1306 display = Adafruit_SSD1306();

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

Packet theData;            // unpacked data
Packet_Packed packedData;  // actual data we recieve

uint8_t data[] = "ok"; // for ACK message
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];     // read buffer

unsigned long previousMillis = 0;     // will store last time interval called
unsigned long currentMillis;
const long interval = 4000;           // milliseconds
char status[20];

// call when recieving a message
void trafficOn() {
  digitalWrite(LED, HIGH);
}

// call when finished with message
void trafficOff() {
  digitalWrite(LED, LOW);
}

void statusStart() {
  //pixels.setBrightness(80);  
  pixels.setPixelColor(0, pixels.Color(0, 0, 150));
  pixels.show();
}

void statusGood() {
  //pixels.setBrightness(30);
  //pixels.clear();  
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.show();
  Serial.println("good");
}

void statusError() {
  // pixels.setBrightness(30);
  pixels.clear();  
  pixels.setPixelColor(0, pixels.Color(150, 0, 0));
  pixels.show();
  Serial.println("bad");
}

void setupOled() {
  if (OLED) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    Serial.println("OLED begun");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Reciever RFM69");
    display.display();
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  
  pixels.begin();
  pixels.setBrightness(80);
  statusStart();

  // ensure start state
  pinMode(LED, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather RFM69HCW Receiver");

  // Hard Reset the RFM module
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  // Initialize radio
  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    strcpy(status, "Radio failed");
    statusError();
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
    strcpy(status, "Freq failed");
    statusError();
  }

  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);

  Serial.print("Listening at "); Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  setupOled();
}

void unpackData() {
  theData.joy_x = packedData.joy_x;
  theData.joy_y = packedData.joy_y;

  theData.joy_button = BB_READ(packedData.buttons, BB_BUTTON_JOY);
  theData.green_button = BB_READ(packedData.buttons, BB_BUTTON_GREEN);
  theData.blue_button = BB_READ(packedData.buttons, BB_BUTTON_BLUE);
}

// print the data we are sending
void printJoystick() {
  if (DEBUG) {
    Serial.print("Recieved... ");
    Serial.print("joy_x: ");
    Serial.print(theData.joy_x);
    Serial.print(" joy_y: ");
    Serial.print(theData.joy_y);
    Serial.print(" joy_buton: ");
    Serial.print(theData.joy_button);
    Serial.print(" green_button: ");
    Serial.print(theData.green_button);
    Serial.print(" blue_button: ");
    Serial.println(theData.blue_button);        
  }
}

void Blink(byte PIN, byte DELAY_MS, byte loops)
{
  for (byte i=0; i<loops; i++)
  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}

float getBatVoltage() {
  float vbat = analogRead(VBAT_PIN);
  vbat *= 2;
  vbat *= 3.3;
  vbat /= 1024;
  return vbat;  
}

void printToOled() {
  if (OLED) {
    char charBuf[40];    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever:"); display.print(status);
    sprintf(charBuf, "VBat:%.2fv RSSI:%i", getBatVoltage(), (int16_t)rf69.lastRssi());
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
}

void clearOled() {
  if (OLED) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Reciever RFM69");
    display.display();
  }
}

// we don't need to ping if we just got a response from the radio
void resetInterval() {
  previousMillis = currentMillis;
}

void pingRadio() {
  Serial.print("Ping node ");
  Serial.print(TRANSMITTER);
  trafficOn();
  char radiopacket[8] = "ping";

  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), TRANSMITTER)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string
      Serial.print(" - ");
      Serial.println((char*)buf);
      strcpy(status, "Ping:Ok");     
      statusGood();
    } else {
      Serial.println(" - No reply, is anyone listening?");
      strcpy(status, "Ping:Err");      
      statusError();
    }
  } else {
    Serial.println(" - Sending failed (no ack)");
    strcpy(status, "Ping:Ack");   
    statusError();
  }
  trafficOff();
}

// check if it is time to ping the transmitter node
void checkPingInterval() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    pingRadio();
    printToOled();
  }
}

void loop() {
  //check if something was received (could be an interrupt from the radio)
  if (rf69_manager.available())
  {
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAck(buf, &len, &from)) {
      trafficOn();      
      
      buf[len] = 0; // zero out remaining string
      
      Serial.print("Got packet from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] (");
      Serial.print(len);
      Serial.print(" bytes) : ");
      // Serial.print((char*)buf);

      if (len != sizeof(Packet_Packed)) {
        Serial.println("Invalid packet received, not matching Packet_Packed struct!");
        for (byte i = 0; i < len; i++) {
          Serial.print((char)buf[i]);
        }
        Serial.println("*");
        strcpy(status, "Data:Err");
        statusError();
      }  
      else
      {
        packedData = *(Packet_Packed*)buf;
        unpackData();
        printJoystick();
        printToOled();
        strcpy(status, "Data:Ok");         
        statusGood();
      }
      
      // Send a reply back to the originator client
      if (!rf69_manager.sendtoWait(data, sizeof(data), from)) {
        Serial.println("Sending failed (no ack)");
        strcpy(status, "Data:Ack");
        statusError();
      }
    }
    trafficOff();
    resetInterval();
  }
  checkPingInterval();
}
