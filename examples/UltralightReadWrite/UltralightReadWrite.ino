/*!
 *@file UltralightReadWrite.ino
 *@brief reads the data that has been written.
 *@details  This demo runs on the arduino platform.
 *@        Download this demo to write something into the card which type is Ultralight.
 *@        ,then reads the data that has been written.
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-7-3
 *@url https://github.com/DFRobot/DFRobot_PN532
 */
#include <DFRobot_PN532.h>
#define  block         (5)
#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataWrite[4] = {2, 0, 2, 0};
DFRobot_PN532:: sCard_t NFCcard;

void setup() {
  Serial.begin(115200);
  while (!nfc.begin()) {
    //Wait for NFC module activation
    Serial.println("initial failure");
    delay (1000);
  }
  Serial.println("Waiting for a card......");
}

void loop() {
  /*Check if there is an IC card*/
  uint8_t dataRead[4];
  if (nfc.scan()) {
    /*!
      The Ultralight'S EEPROM memory is organized in pages with 4 bytes per page. The Ultralight
      has 14d pages in total. The memory organization can be seen  in the following table .

             page Adr              BYTE number within a page
                          0           |    1    |   2       |    3      |
              0                       serial  number
              1                       serial  number
              2          serial number|internal |       lock bytes      |
              3           OTP         | OTP     |  OTP      |   OTP     |
              -----------------------------------------------------------
              4
              ...                     user memory
              15
              -----------------------------------------------------------
              16
              17                      Configuration pages
              18
              19                      Counter pages
    */
    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x44 && memcmp(NFCcard.cardType, "Ultralight", 10) == 0) {
      Serial.print("Data to be written(HEX):");
      for (int i = 0; i < 4; i++) {
        Serial.print(dataWrite[i], HEX);
        Serial.print(" ");
      }
      Serial.println("");
      Serial.print("The execution result:");
      if (nfc.writeNTAG(block, dataWrite) == 1) {
        Serial.println("write success");
      }
      else {
        Serial.println("write failure");
      }
      Serial.print("Data read(HEX):");
      if (nfc.readNTAG(dataRead, block) == 1) {

        Serial.print(":");
        for (int i = 0; i < 4; i++) {
          Serial.print(dataRead[i], HEX);
          Serial.print(" ");
        }
        Serial.println("");
      }
      else {
        Serial.println("Read failure");
      }
    }
    else {
      Serial.println("The card type is not Ultralight...");
    }
  }
  else {
    //Serial.println("no card");
  }
  delay(2000);
}

