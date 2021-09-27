#include <Arduino.h>

#define CWW_IS_LORA true

#define NETWORKID     100
#define CWW_RADIO_FREQ    915.0

/* for Feather radio */
#define RADIO_CS      10
#define RADIO_INT     6
#define RADIO_RST     11

#if CWW_IS_LORA == true
const char RADIO_TYPE[] = "RF95";
#define CWW_RADIO_POWER   23
#define CWW_IS_RFM69HCW   false
#else
const char CWW_RADIO_TYPE[] = "RF86";
#define CWW_RADIO_POWER   20
#define CWW_IS_RFM69HCW   true
// The encryption key has to be the same as the one in the server
uint8_t ENCRYPTION_KEY[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
#endif

#define SERIAL_BAUD   115200
