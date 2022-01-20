/*!
 *@file MifareClassicReadWrite.ino
 *@brief read data and write data to nfc card
 *@details  This demo runs on the arduino platform.
 *@        Download this demo to learn how to wirte data to card.
 *@        We can read the data on the card to see if the write is successful.
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-7-3
 *@url https://github.com/DFRobot/DFRobot_PN532
 */
#include <DFRobot_PN532.h>

#define BLOCK_SIZE      16
#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataWrite[BLOCK_SIZE] = {"DFRobot NFC"};
uint8_t dataRead[16] = {0};
DFRobot_PN532:: sCard_t NFCcard;

void setup() {

  Serial.begin(115200);
  while (!nfc.begin()) {
    Serial.println("initial failure");
    delay (1000);
  }
  Serial.println("Waiting for a card......");
}
void loop() {
  //Scan, write and read NFC card every 2s
  //Print all what is to be written and read
  if (nfc.scan()) {
    NFCcard = nfc.getInformation();
    if ((NFCcard.AQTA[1] == 0x02 || NFCcard.AQTA[1] == 0x04)) {
      Serial.print("Data to be written(string):");
      Serial.println((char *)dataWrite);
      Serial.print("Data to be written(HEX):");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(dataWrite[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      //Write data(16 bytes) to sector 1
      if (nfc.writeData(2, dataWrite) != 1) {
        Serial.println("write failure!");
      }

      //Read sector 1 to verify the write results
      nfc.readData(dataRead, 2);
      Serial.print("Data read(string):");
      Serial.println((char *)dataRead);
      Serial.print("Data read(HEX):");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(dataRead[i], HEX);
        Serial.print(" ");
        dataRead[i] = 0;
      }
      Serial.println();
    }
    else {
      Serial.println("The card type is not mifareclassic...");
    }
  }
  else {
    //Serial.println("no card");
  }
  delay(2000);
}
