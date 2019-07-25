
/*!
    @flie NFC_cardinfo.ino
    @Copyright   [DFRobot](http://www.dfrobot.com), 2016
    @Copyright   GNU Lesser General Public License
    @version  V1.0
    @date  07/03/2019

    @brief This demo runs on the arduino platform.
           Download this demo to read the basic information of the card,
           including UID, manufacturer, storage space, RF technology etc.
           
           Suported NFC card/tag:
           1.MIFARE Classic S50/S70
           2.NTAG213/215/216
           3.MIFARE Ultralight

    This demo and related libraries are for DFRobot Gravity: I2C&UART NFC Module
    Product(CH): http://www.dfrobot.com.cn/goods-762.html
    Product(EN): https://www.dfrobot.com/product-892.html
*/
#include <DFRobot_PN532.h>

#define  BLOCK_SIZE      16
#define  PN532_IRQ      (2)
#define  INTERRUPT      (1)
#define  POLLING        (0)
// Use this line for a breakout or shield with an I2C connection
// Check the card by polling
DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING); 
struct card NFCcard;

void setup() {

  Serial.begin(115200);
  //Initialize the NFC module
  while (!nfc.begin()) {
    Serial.println("initial failure");
    delay (1000);
  }        
  Serial.println("Please place the NFC card/tag on module..... ");
}

void loop() {
  //Scan, write and read NFC card every 2s
  //Print all what is to be written and read
  
  if (nfc.scan()) {
    //Read the basic information of the card
    NFCcard = nfc.getInformation();
    Serial.println("----------------NFC card/tag information-------------------");
    Serial.print("UID Lenght: "); Serial.println(NFCcard.uidlenght);
    Serial.print("UID: ");
    for (int i = 0; i < NFCcard.uidlenght; i++) {
      Serial.print(NFCcard.uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
    Serial.print("AQTA: "); Serial.print("0x"); Serial.print("0"); Serial.print(NFCcard.AQTA[0], HEX); Serial.print(""); Serial.println(NFCcard.AQTA[1], HEX);
    Serial.print("SAK: "); Serial.print("0x"); Serial.println(NFCcard.SAK, HEX);
    Serial.print("Type: "); Serial.println(NFCcard.cardType);
    Serial.print("Manufacturer:"); Serial.println(NFCcard.Manufacturer);
    Serial.print("RF Technology:"); Serial.println(NFCcard.RFTechnology);
    Serial.print("Memory Size:"); Serial.print(NFCcard.size); Serial.print(" bytes(total)/"); Serial.print(NFCcard.usersize); Serial.println(" bytes(available)");
    Serial.print("Block/Page Size:"); Serial.print(NFCcard.blockSize); Serial.println(" bytes");
    //Serial.print("Sector Size:"); Serial.print(NFCcard.sectorSize); Serial.println(" bytes");
    Serial.print("Number of Blocks/pages:"); Serial.println(NFCcard.blockNumber);
  }
  else {
    //Serial.println("no card!");
  }
  delay(2000);
}
