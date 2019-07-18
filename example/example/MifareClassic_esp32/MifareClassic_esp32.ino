
/*!
    MifareClassic_esp32.ino
    This demo  runs on the mPython platform
    Download this demo to learn how to use this module on esp32
    Data will print on your serial monitor

    Product: http://www.dfrobot.com.cn/goods-762.html
    Copyright   [DFRobot](http://www.dfrobot.com), 2016
    Copyright   GNU Lesser General Public License

    version  V1.0
    date  07/03/2019
*/

#include <DFRobot_PN532.h>

#define BLOCK_SIZE      16


DFRobot_PN532_UART  nfc;  //Use abbreviations instead of full names
uint8_t dataWrite[BLOCK_SIZE] = {"DFRobot NFC"};   /*Write data page */
uint8_t dataRead[16] = {0};
struct card NFCcard;

void setup() {

  Serial.begin(115200);
  while (!nfc.begin(&Serial1, P0, P1)) {
    Serial.print("initial failure");
    delay (1000);
  }     /*Initialize parameters, select soft serial port pin (RX,TX)*/
  Serial.println("Waiting for a card......");

}
void loop() {
  //Scan, write and read NFC card every 5s
  //Print all what is to be written and read
  //char data[16];
  //struct card card;
  //card = nfc.getInformation();
  //Serial.println("Waiting for a card......");
  delay(2000);
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
        //data[i] =  dataRead[i];
        Serial.print(dataRead[i], HEX);
        Serial.print(" ");
        dataRead[i] = 0;
      }
      //Serial.println( nfc.readData(1));
      Serial.println();
      delay(3000);
    }
    else {
      Serial.println("The card type is not mifareclassic...");
    }
  }
  else {
    Serial.println("no card");
  }

  delay(3000);
}