/*!
 *@file NTAG21xReadAllmemory.ino
 *@brief read data from NTAG card
 *@details  This demo runs on the arduino platform.
 *@        Download this demo to read all memories in NTAG'S memory.
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-7-3
 *@url https://github.com/DFRobot/DFRobot_PN532
 */
#include <DFRobot_PN532.h>

#define PN532_IRQ       (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataRead[4];
DFRobot_PN532:: sCard_t NFCcard;

//This procedure is to print the corresponding page.
void printi(uint8_t block) {
  //uint8_t data[4];
  Serial.print(":");
  if (nfc.readNTAG(dataRead, block) == 1) {
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
    //Print out everything in memory
    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x44 && memcmp(NFCcard.cardType, "Ultralight", 10) != 0) {
      Serial.println("----------------Here is the card information to read-------------------");
      for (int i = 0; i < NFCcard.blockNumber; i++) {
        Serial.print("block "); Serial.print(i);
        if (i == 0 || i == 1 || i == 2) {
          //Serial.print("Block ");printi(i);
          Serial.print("(Manufacturer data and lock bytes)--------------->");
          printi(i);
        }
        else if (i == 3) {
          Serial.print("(Capability Container)--------------------------->");
          printi(i);
        }
        else if (i == NFCcard.blockNumber - 5) {
          Serial.print("(Dynamic lock bytes)----------------------------->");
          printi(i);
        }
        else if (i > (NFCcard.blockNumber - 5) && i <  NFCcard.blockNumber) {
          Serial.print("(Configuration pages)---------------------------->");
          printi(i);
        }
        else {
          Serial.print("------------------------------------------------->");
          printi(i);
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
