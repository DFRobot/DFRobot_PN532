/*!
    @flie NTAG21x_ClearAllMemory.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief his demo runs on the arduino platform
           Download this demo to View everything in memory which type is NTAG21x.

    This demo and related libraries are for DFRobot Gravity: I2C NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/

#include <DFRobot_PN532.h>


// the IRQ and reset lines.  Use the value for the shield!
#define NTAG213_LENGTH      (39)
#define NTAG215_LENGTH      (129)
#define NTAG216_LENGTH      (226)



#define PN532_IRQ      (4)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);  //Use abbreviations instead of full names
//uint8_t dataRead = {0};
uint8_t data[4] = {0};
struct card NFCcard ;

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
  //uint8_t data[4] = {2, 0, 1, 9};
  ///struct card card ;
  //Serial.println("Waiting for a card......");
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
    Serial.println("no card");
  }

  delay(10000);
}
//This procedure is to print the corresponding module of I.