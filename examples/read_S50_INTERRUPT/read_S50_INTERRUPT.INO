/*!
    @flie read_S50_INTERRUPT.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief This demo runs on the arduino uno platform
           Download this demo to learn how to read data on card and read data by interrupt.

           We can read the data on the card to see if the write is successful

    This demo and related libraries are for DFRobot Gravity: I2C&UART NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/
#include <DFRobot_PN532.h>

#define BLOCK_SIZE      16
#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
//use this line for a breakout or shield with an I2C connection
//check the card by interruption
DFRobot_PN532_IIC  nfc(PN532_IRQ, INTERRUPT);  
uint8_t dataWrite[BLOCK_SIZE] = {"DFRobot NFC"}; 
uint8_t dataRead[16] = {0};
struct card NFCcard ;

void setup() {
  Serial.begin(115200);
  while (!nfc.begin()) {
    Serial.println("initial failure");
    delay (1000);
  }
  Serial.println("Waiting for a card......");
}

void loop() {
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
        dataRead[i] = 0;
      }
    }
    else {
      Serial.println("The card type is not mifareclassic...");
    }
  }
  else {
    //Serial.println("no card");
  }    
  delay(1000);
}
