#include <Arduino.h>

/*
  Using RadioHead RFM69 library
*/

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include "status.h"
#include "utils.h"
#include "controls.h"
#include "screen.h"
#include "main.h"

#define NODEID        1  
#define TRANSMITTER   2

#define DEBUG         true
#define OLED          true
#define VBAT_PIN      A6

int16_t packetnum = 0;  // packet counter, we increment per xmission

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);
// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, NODEID);

// Packet theData;            // unpacked data
// Packet_Packed packedData;  // actual data we recieve

uint8_t data[] = "ok"; // for ACK message
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];     // read buffer

unsigned long previousMillis = 0;     // will store last time interval called
unsigned long currentMillis;
const long interval = 4000;           // milliseconds
char status[20];

void setup() {
  Serial.begin(SERIAL_BAUD);
  initStatus();

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
  rf69.setEncryptionKey(ENCRYPTION_KEY);

  Serial.print("Listening at "); Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  initOled();
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
      statusOk();
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
    printToOled(status, getBatVoltage(), rf69.lastRssi(), getData());
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
        setPackedData(*(Packet_Packed*)buf);
        unpackData();
        printJoystick();
        printToOled(status, getBatVoltage(), rf69.lastRssi(), getData());
        strcpy(status, "Data:Ok");         
        statusOk();
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
