
/*!
    @flie MifareClassic_ReadAllMemory.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief his demo runs on the arduino platform
           Download this demo to View everything in CARD which type is mifareclassic

    This demo and related libraries are for DFRobot Gravity: I2C NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/

#include <DFRobot_PN532.h>

#define PN532_IRQ      (4)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);  //Use abbreviations instead of full names

uint8_t dataRead[16] = {0};
struct card NFCcard ;
//This procedure is to print the corresponding module of I.
void printi(uint8_t block) {
  //uint8_t data[16];
  if (nfc.readData(dataRead, block) == 1) {               /*Read 16 data in module 1 of IC card*/
    //Serial.println("Data read(HEX):");
    for (int i = 0; i < 16; i++) {
      Serial.print(dataRead[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
  else {
    Serial.println("locked");
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
  //uint8_t data[16];
  //struct card card ;
  //Wait for a card that supports the ISO/IEC1443-3,Type A standard
  //
  //Serial.println("Waiting for a card......");
  delay(2000);
  if (nfc.scan()) {
    /*!
      The 1024 × 8 bit EEPROM memory is organized in 16 sectors of 4 blocks. One block
      contains 16 bytes.
      Sector  0   block 0           (Manufacturer   block)              Manufacturer  Data
                  block 1           (data)
                  block 2           (data)
                  block 3           (Sector Trailer 0)                  keyA   Access Bits   KeyB
      Sector  1       block 4           (data)
                  block 5           (data)
                  block 6           (data)
                  block 7           (Sector Trailer 1)                  keyA   Access Bits   KeyB
      ...
      ...
      ...
      Sector 15       block 60           (data)
                  block 61           (data)
                  block 62           (data)
                  block 63           (Sector Trailer 15)                  keyA   Access Bits   KeyB
    */
    //Print out everything in memory
    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x02 || NFCcard.AQTA[1] == 0x04) {
      Serial.println("----------------Here is the card information to read-------------------");
      for (int i = 0; i < NFCcard.blockNumber; i++) {
        if (i == 0) {
          Serial.print("Block "); Serial.print(i);
          Serial.print(":UID0-UID3/MANUFACTURER--------------->");
          printi(i);
        }
        else if ((i + 1) % 4 == 0 && i < 128 ) {
          Serial.print("Block "); Serial.print(i);
          Serial.print(":KEYA/ACCESS/KEYB--------------------->");
          printi(i);
        }
        else if ((i + 1) % 16 == 0 && i > 127) {
          Serial.print("Block "); Serial.print(i);
          Serial.print(":KEYA/ACCESS/KEYB--------------------->");
          printi(i);
        }
        else {
          Serial.print("Block "); Serial.print(i);
          Serial.print(":DATA   ------------------------------>");
          printi(i);
        }
      }
    }
    else {
      Serial.println("The card type is not mifareclassic...");
    }

  }
  else {
    Serial.print("no card");
  }
  delay(2000);
}