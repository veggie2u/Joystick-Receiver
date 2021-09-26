#ifndef RADIO_DATA_H
#define RADIO_DATA_H

#define NETWORKID     100
#define RF69_FREQ     915.0
#define IS_RFM69HCW   true

/* for Feather radio */
#define RFM69_CS      10
#define RFM69_INT     6
#define RFM69_RST     11

// The encryption key has to be the same as the one in the server
uint8_t ENCRYPTION_KEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
                  
#define SERIAL_BAUD   115200

enum Controls {
  redButton,
  greenButton,
  yellowButton,
  blueButton
};

#endif
