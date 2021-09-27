#include <Arduino.h>

/*
  Radio Receiver. Listens for joystick/buttons.
  Pings receiver to make sure it is still around.
*/

#include <SPI.h>
#include "main.h"
#if CWW_IS_LORA == true
#include <RH_RF95.h>
const int MAX_MESSAGE_LEN = RH_RF95_MAX_MESSAGE_LEN;
RH_RF95 radio(RADIO_CS, RADIO_INT);
#else
#include <RH_RF69.h>
const int MAX_MESSAGE_LEN = RH_RF69_MAX_MESSAGE_LEN;
RH_RF69 radio(RADIO_CS, RADIO_INT);
#endif
#include <RHReliableDatagram.h>
#include <utils.h>
#include <debug.h>
#include <status.h>
#include <controls.h>
#include <screen.h>

#define NODEID        1  
#define TRANSMITTER   2

#define VBAT_PIN      A6

int16_t packetnum = 0;  // packet counter, we increment per xmission

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(radio, NODEID);

uint8_t data[] = "ok"; // for ACK message
uint8_t buf[MAX_MESSAGE_LEN];     // read buffer

unsigned long previousMillis = 0;     // will store last time interval called
unsigned long currentMillis;
const long interval = 4000;           // milliseconds
char status[20];
char charBuf[100]; // for sprintf

void setup() {
  Serial.begin(SERIAL_BAUD);
  initStatus();
  initOled(RADIO_TYPE);
  delay(2000);

  sprintf(charBuf, "Feather %s Receiver", RADIO_TYPE);
  debuglnD(charBuf);
 
  pinMode(RADIO_RST, OUTPUT);

  // Hard Reset the radio module
  #if CWW_IS_LORA == true
  digitalWrite(RADIO_RST, HIGH);
  delay(10);
  digitalWrite(RADIO_RST, LOW);
  delay(10);
  digitalWrite(RADIO_RST, HIGH);
  delay(10);
  #else
  digitalWrite(RADIO_RST, LOW);
  delay(10);
  digitalWrite(RADIO_RST, HIGH);
  delay(10);
  digitalWrite(RADIO_RST, LOW);
  delay(10);
  #endif
  
  // Initialize radio
  if (!manager.init()) {
    debuglnD("RFM69 radio init failed");
    strcpy(status, "Radio failed");
    statusError();
    printErrorOled(status);
    while (1) {
      trafficOff();
      delay(500);
      trafficOn();
      delay(500);
    }
  }
  sprintf(charBuf, "%s radio init OK!", RADIO_TYPE);
  debuglnD(charBuf);
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  if (!radio.setFrequency(CWW_RADIO_FREQ)) {
    debuglnD("setFrequency failed");
    strcpy(status, "Freq failed");
    printErrorOled(status);
    statusError();
  }

  radio.setTxPower(CWW_RADIO_POWER, CWW_IS_RFM69HCW);  // range from 14-20 for power, 2nd arg must be true for 69HCW
  
  #if CWW_IS_LORA == false
  radio.setEncryptionKey(ENCRYPTION_KEY);
  #endif

  sprintf(charBuf, "Listening on %s readio @ %.1f Mhz", RADIO_TYPE, CWW_RADIO_FREQ);
  debuglnD(charBuf);
}

float getBatVoltage() {
  float vbat = analogRead(VBAT_PIN);
  vbat *= 2;
  vbat *= 3.3;
  vbat /= 1024;
  return vbat;  
}

// we don't need to ping if we just got a response from the radio
void resetInterval() {
  previousMillis = currentMillis;
}

void pingRadio() {
  debugD("Ping node ");
  debugD(TRANSMITTER);
  trafficOn();
  char radiopacket[8] = "ping";

  if (manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), TRANSMITTER)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string
      debugD(" - ");
      debuglnD((char*)buf);
      strcpy(status, "Ping:Ok");     
      statusOk();
    } else {
      debuglnD(" - No reply, is anyone listening?");
      strcpy(status, "Ping:Err");      
      statusError();
    }
  } else {
    debuglnD(" - Sending failed (no ack)");
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
    printToOled(status, getBatVoltage(), radio.lastRssi(), getData());
  }
}

void loop() {
  //check if something was received (could be an interrupt from the radio)
  if (manager.available())
  {
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from)) {
      trafficOn();      
      
      buf[len] = 0; // zero out remaining string
      
      sprintf(charBuf, "Got packet from #%i [RSSI : %i] (%i) bytes", from, radio.lastRssi(), len);
      debuglnD(charBuf);

      if (len != sizeof(Packet_Packed)) {
        Serial.println("Invalid packet received, not matching Packet_Packed struct!");
        for (byte i = 0; i < len; i++) {
          debugD((char)buf[i]);
        }
        debuglnD("*");
        strcpy(status, "Data:Err");
        statusError();
      }  
      else
      {
        setPackedData(*(Packet_Packed*)buf);
        unpackData();
        printJoystick(getData());
        printToOled(status, getBatVoltage(), radio.lastRssi(), getData());
        strcpy(status, "Data:Ok");         
        statusOk();
      }
      
      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from)) {
        debuglnD("Sending failed (no ack)");
        strcpy(status, "Data:Ack");
        statusError();
      }
    }
    trafficOff();
    resetInterval();
  }
  checkPingInterval();
}
