/*!
 *@file readS50Uart.ino
 *@brief read data through serial ports.
 *@details  This demo runs on the Arduino MEGA2560 platform.
 *@        Download this demo to learn how to read data on card and read data through serial ports.
 *@        Read the data on the card to see if the write is successful.
 *@copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-7-3
 *@url https://github.com/DFRobot/DFRobot_PN532
 */
#include <DFRobot_PN532.h>

#define BLOCK_SIZE 16
//Initialize MEGA2560
DFRobot_PN532_UART  nfc;
struct card NFCcard ;

void setup() {
  Serial.begin(115200);
  while (!nfc.begin(&Serial3)) {
    Serial.println("initial failure");
    delay (1000);
  }
  Serial.println("Waiting for a card......");
}

uint8_t dataWrite[BLOCK_SIZE] = {"DFRobot NFC"};   /*Write data page */
uint8_t dataRead[16] = {0};

void loop() {
  //Scan, write and read NFC card every 2s
  //Print all what is to be written and read

  if (nfc.scan() == true) {

    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x02 || NFCcard.AQTA[1] == 0x04) {
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
        //data[i] =  dataRead[i];
        Serial.print(dataRead[i], HEX);
        Serial.print(" ");
      }
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
