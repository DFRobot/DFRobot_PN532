/*!
  * iic_uart_read.ino
  *
  * This demo  runs on the arduino platform
  * Download this demo to learn how to read data on card
  * Use the scanning function to scan the IC card
  *
  * Product: http://www.dfrobot.com.cn/goods-762.html
  * Copyright   [DFRobot](http://www.dfrobot.com), 2016
  * Copyright   GNU Lesser General Public License
  *
  * version  V1.0
  * date  07/03/2019
  */
#include <DFRobot_nfc.h>
/*           //Initialize mega2560 
DFRobot_PN532_UART  nfc;      
void setup() {
    nfc.begin(&Serial1);
    Serial.begin(9600);
}*/
DFRobot_PN532_IIC  nfc;    //Use abbreviations instead of full names 

void setup() {
    Serial.begin(9600);
    nfc.begin();
}
void loop() {
       /*Check if there is an IC card*/
    if(nfc.scan()){
      Serial.println(nfc.readData(1));               /*Read 16 data in module 1 of IC card*/
      Serial.println(nfc.readData(1,1));             /*Read the first of 16 data in sector1 of IC card*/
      Serial.println(nfc.readUid());                 /*Identify the id of the card*/
    
      if(nfc.scan("fb7c6083")){ 
        Serial.print("card 1");
        Serial.println(nfc.readUid());                   /*Print out the ID of the IC card*/
        Serial.println("");
                              }
                  }
    delay(10000);
}
