
/*!
    @flie MifareClassic_ClearAllMemory.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief This demo runs on the arduino platform.
           Download this demo to clear all data areas.
           We can read the data on the card to see if the write is successful.

    This demo and related libraries are for DFRobot Gravity: I2C&UART NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/
#include <DFRobot_PN532.h>

#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t data[16] = {0};
struct card NFCcard;

void setup() {
  Serial.begin(115200);
  while (!nfc.begin()) {
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
      The 1024 × 8 bit EEPROM memory is organized in 16 sectors of 4 blocks. One block
      contains 16 bytes.
      Sector  0    block 0           (Manufacturer   block)              Manufacturer  Data
                   block 1           (data)
                   block 2           (data)
                   block 3           (Sector Trailer 0)                  keyA   Access Bits   KeyB
      Sector  1    block 4           (data)
                   block 5           (data)
                   block 6           (data)
                   block 7           (Sector Trailer 1)                  keyA   Access Bits   KeyB
      ...
      ...
      ...
      Sector 15    block 60           (data)
                   block 61           (data)
                   block 62           (data)
                   block 63           (Sector Trailer 15)                  keyA   Access Bits   KeyB
    */
    NFCcard = nfc.getInformation();
    if (nfc.scan() == true && (NFCcard.AQTA[1] == 0x02 || NFCcard.AQTA[1] == 0x04)) {
      Serial.println("Clear all the data in the card...");
      for (int i = 0; i < NFCcard.blockNumber; i++) {
        if (i == 0) {
          Serial.print("block:"); Serial.print(i); Serial.println(" Producer area, it has been written dead");
        }
        else if ((i + 1) % 4 == 0 && i < 128) {
          Serial.print("block:"); Serial.print(i); Serial.println(" Key area, please modify carefully");
        }
        else if ((i + 1) % 16 == 0 && i > 128) {
          Serial.print("block:"); Serial.print(i); Serial.println(" Key area, please modify carefully");
        }
        else {
          Serial.print("block:"); Serial.print(i);
          if (nfc.writeData(i, data))   Serial.println(" clear success");
          else Serial.println(" clear failure");
        }
      }
    }
    else {
      Serial.println("The card type is not mifareclassic...");
    }
  }
  else {
    //Serial.println("no card!");
  }
  delay(2000);
}
