/*!
  * arduino_write.ino
  *
  * This demo  runs on the arduino platform
  * Download this demo to learn how to wirte data to card
  * We can read the data on the card to see if the write is successful
  * 
  * Product: http://www.dfrobot.com.cn/goods-762.html
  * Copyright   [DFRobot](http://www.dfrobot.com), 2016
  * Copyright   GNU Lesser General Public License
  *
  * version  V1.0
  * date  07/03/2019
  */

#include <DFRobot_nfc.h>

/*             //Inintialize mega2560
DFRobot_PN532_UART  nfc;
void setup() {
    nfc.begin(&Serial1);
    Serial.begin(9600);
}*/
DFRobot_PN532_IIC  nfc;   //Use abbreviations instead of full names 
void setup() {
    Serial.begin(9600);
    nfc.begin();        /*Initialize the NFC module*/
}
void loop() {
  uint8_t data[16]={0};     /*Write data page */ 
    
    
  if(nfc.scan()){
    nfc.writeData(1,data);     /*Write 16 pieces of data to sector 1*/
    nfc.writeData (1,1,6);                 /*Sector 1 should be replaced by 6*/
    Serial.println(nfc.readData(1));    /*Read sector1 data to verify the write result*/
                }
    delay(10000);
}
