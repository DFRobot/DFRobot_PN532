
/*!
    @flie Ultralight_ClearAllMemory.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief This demo runs on the arduino platform.
           Download this demo to Clear all data areas.
           We can read the data on the card to see if the write is successful.


    This demo and related libraries are for DFRobot Gravity: I2C&UART NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/


#include <DFRobot_PN532.h>

// the IRQ and reset lines.  Use the value for the shield!
#define PN532_IRQ       (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t data[4] = {0};
struct card NFCcard ;

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
  //Initializes the values of all data blocks, setting them all to zero.
  if (nfc.scan()) {
    NFCcard = nfc.getInformation();
    if (NFCcard.AQTA[1] == 0x44 && memcmp(NFCcard.cardType, "Ultralight", 10) == 0) {
      for (int i = 0; i < 20 ; i++) {
        Serial.print("BLOCK "); Serial.print(i); Serial.print(":");
        if (i < 4 || i > NFCcard.blockNumber - 5) {
          Serial.println("Can not be changed");
        }
        else {
          if (nfc.writeUltralight(i, data) == 1) {
            Serial.println(" clear success");
          }
          else {
            Serial.println(" clear failure");
          }
        }
      }
    }
    else {
      Serial.println("The card type is not Ultralight...");
    }
  }
  else {
    //Serial.println("no card!");
  }
  delay(2000);
}
