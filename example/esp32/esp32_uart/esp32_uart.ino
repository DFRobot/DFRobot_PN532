
/*!
  * esp32_uart.ino
  * This demo  runs on the mPython platform
  * Download this demo to learn how to use this module on esp32
  * Data will print on your serial monitor
  *
  * Product: http://www.dfrobot.com.cn/goods-762.html
  * Copyright   [DFRobot](http://www.dfrobot.com), 2016
  * Copyright   GNU Lesser General Public License
  *
  * version  V1.0
  * date  07/03/2019
  */

#include <DFRobot_nfc.h>

DFRobot_PN532_UART  nfc; 

void setup() {
    nfc.begin(&Serial1,P0,P1);      /*Initialize parameters, select soft serial port pin (RX,TX)*/
    Serial.begin(9600);
}
void loop() {
    /*Check if there is an IC card*/
    if(nfc.scan()){

       Serial.println(nfc.readData(1));     /*Read 16 data in module 1 of IC card*/

       Serial.println(nfc.readData(1,1));   /*Read the first of 16 data in sector1 of IC card*/
    
       if(nfc.scan("fb7c6083")){             /*Identify the card ID*/
   
          Serial.println("card 1");
          Serial.print("Card id:");
          Serial.println(nfc.readUid());      /*Print out the ID of the IC card*/
     }
   }
    delay(10000);
}
