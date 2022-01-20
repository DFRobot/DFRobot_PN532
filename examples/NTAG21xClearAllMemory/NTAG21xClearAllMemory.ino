/*!
 *@file NTAG21xClearAllMemory.ino
 *@brief clear all data areas on NTAG21x card
 *@details  This demo runs on the arduino platform
 *@         Download this demo to clear all memories which type is NTAG21x.
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-7-3
 *@url https://github.com/DFRobot/DFRobot_PN532
 */
#include <DFRobot_PN532.h>

// the IRQ and reset lines. Use the value for the shield!
#define PN532_IRQ       (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);

uint8_t data[4] = {0};
DFRobot_PN532:: sCard_t NFCcard;

void setup() {
  Serial.begin(115200);
  while (!nfc.begin()) {
    //Wait for NFC module activation
    Serial.println("initial failure");
    delay (1000);
  }
  Serial.println("Clear all the data in the card.......");
  Serial.println("Waiting for a card......");
}
void loop() {
  /*Check if there is an IC card*/
  if (nfc.scan()) {
    /*!
        The NTAG's EEPROM memory is organized in pages with 4 bytes per page. NTAG213 variant has
        45 pages, NTAG215 variant has 135 pages and NTAG216 variant has 231 pages in total.
        The memory organization can be seen  in the following table .

                page Adr      BYTE number within a page
                              0            |    1    |   2       |    3      |
                   0                       serial  number
                   1                       serial  number
                   2          serial number|internal | lock bytes| lock bytes|
                   3                       Capability Container(cc)
                   ----------------------------------------------------------------
                   -----------------------NTAG213----------------------------------
                   4
                   ...                     user memory
                   39
                   ----------------------------------------------------------------
                   ------------------------NTAG215---------------------------------
                   4
                   ...                     user memory
                   129
                   ----------------------------------------------------------------
                   ------------------------NTAG216---------------------------------
                   4
                   ...                     user memory
                   225
    */
    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x44 && memcmp(NFCcard.cardType, "Ultralight", 10) != 0) {
      Serial.println("---------------------------Here is the clear status-----------------------------");
      for (int i = 0; i < NFCcard.blockNumber; i++) {
        Serial.print("BLOCK ");
        Serial.print(i); Serial.print(":");
        if (i < 4 || i > NFCcard.blockNumber - 6) {
          Serial.println(" Can not be changed");
        }
        else if (nfc.writeNTAG(i, data) == 1) {

          Serial.println(" clear success");
        }
        else {
          Serial.println(" clear failure");
        }
      }
    }
    else {
      Serial.println("The card type is not NTAG21x...");
    }
  }
  else {
    //Serial.println("no card");
  }
  delay(2000);
}

