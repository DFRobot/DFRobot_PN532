#include"DFRobot_PN532.h"
#include <Wire.h>
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>
#define PN532_PACKBUFFSIZ 64
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
uint8_t DFRobot_PN532::getUltraversion(uint8_t block){
    if(!this->nfcEnable)
        return -1;
    if(!scan())
        return -1;
    unsigned char cmdRead[4];
    unsigned char sum = 0,count = 0;
        cmdRead[0] = COMMAND_INDATAEXCHANGE;
        cmdRead[1] = 1;                   /* Card number */
        cmdRead[2] = CARD_CMD_READING;     /* NTAG Read command = 0x30 */
        cmdRead[3] = block; 
    
    writeCommand(cmdRead,4);
    if(!readAck(32))
        return -1;
    if(receiveACK[12] == 0x41 && receiveACK[13] == 0x00){
    }
    else
        return -1;
    return 1;
}
uint8_t DFRobot_PN532::readNTAG(uint8_t *buffer,uint8_t block){
    if(block > 231)
        return -1;
    if(!this->nfcEnable)
        return -1;
    if(!scan())
        return -1;
    unsigned char cmdRead[4];
    unsigned char sum = 0,count = 0;
        cmdRead[0] = COMMAND_INDATAEXCHANGE;
        cmdRead[1] = 1;                   /* Card number */
        cmdRead[2] = CARD_CMD_READING;     /* NTAG Read command = 0x30 */
        cmdRead[3] = block; 
    
    writeCommand(cmdRead,4);
    if(!readAck(32))
        return -1;
    String dataSrt = "";
    if(receiveACK[12] == 0x41 && receiveACK[13] == 0x00){
        for(uint8_t i = 0;i<4;i++){
            buffer[i] = receiveACK[14 + i];
               }
    }
    else
        return -1;
    
    return 1;
}
     
bool  DFRobot_PN532::writeNTAG(int block, uint8_t data[]){
    if(block > 225 || block < 4)
        return false;
    if(!this->nfcEnable)
        return false;
    if(!this->scan())
        return false;
    unsigned char cmdWrite[20];
        cmdWrite[0] = COMMAND_INDATAEXCHANGE;
        cmdWrite[1] = 1;                      /* Card number */
        cmdWrite[2] = CARD_CMD_WRITEINGTONTGE;       /* NTAG Write command = 0xA2 */
        cmdWrite[3] = block;
    unsigned char sum=0,count=0;
    for(int i = 4;i < 8;i++) cmdWrite[i]=data[i - 4];// Data to be written
    this->writeCommand(cmdWrite,8);

    this->readAck(16);
    return true;

}
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
uint8_t DFRobot_PN532::readUltralight(uint8_t *buffer,uint8_t block){
    if(block > 41)
      return -1;
    if(!this->nfcEnable)
        return -1;
    if(!scan())
        return -1;
    unsigned char cmdRead[4];
    unsigned char sum = 0,count = 0;
        cmdRead[0] = COMMAND_INDATAEXCHANGE;
        cmdRead[1] = 1;                   /* Card number */
        cmdRead[2] = CARD_CMD_READING;     /* Ultralight Read command = 0x30 */
        cmdRead[3] = block; 
    
    writeCommand(cmdRead,4);
    if(!readAck(22))
        return -1;
    if(receiveACK[12] == 0x41 && receiveACK[13] == 0x00){
        for(uint8_t i = 0;i<4;i++){
            buffer[i] = receiveACK[14 + i];
        }
               }
    else{
        return -1;
    }
    return 1;
}

bool DFRobot_PN532::writeUltralight(int block, uint8_t data[]){
    if( block < 4)
        return false;
    if(!this->nfcEnable)
        return false;
    if(!this->scan())
        return false;
    unsigned char cmdWrite[20];
        cmdWrite[0] = COMMAND_INDATAEXCHANGE;
        cmdWrite[1] = 1;                      /* Card number */
        cmdWrite[2] = CARD_CMD_WRITEINGTOULTRALIGHT;       /* Ultralight Write command = 0xA2 */
        cmdWrite[3] = block;
    unsigned char sum=0,count=0;
    for(int i = 4;i < 8;i++) cmdWrite[i]=data[i - 4];// Data to be written
    this->writeCommand(cmdWrite,8);
    this->readAck(16);
    return true;

}



/*!The PASSWORD will be checked once the card is read or written,
   the password of the card usually set by the manufacturer,
   the default passWord is 0xff 0xff 0xff 0xff 0xff 0xff.
*/


bool DFRobot_PN532::passWordCheck(int block,uint8_t id[],uint8_t st[])
{   //bool success = false;
    if(!this->nfcEnable)
        return false;
    unsigned char cmdPassWord[14];
    cmdPassWord[0] = COMMAND_INDATAEXCHANGE;   /* Data Exchange Header */
    cmdPassWord[1] = 1;                              /* The quantity number of the maxium card that can be detected in every research*/
    cmdPassWord[2] = 0x60;                          
    cmdPassWord[3] = block;
    unsigned char sum=0,count=0;
    for(int i = 4;i < 10;i++) cmdPassWord[i] = st[i - 4];              // PassWord
    for(int i = 10;i < 14;i++) cmdPassWord[i] = id[i - 10];           // nfcUid
    this->writeCommand(cmdPassWord,14);     /*!Send a series of commands to the chip*/ 
    
    if(!this->readAck(16))
        return false;
    //for(int i= 0 ; i<32 ;i++){
    //  Serial.print(receiveACK[i],HEX);
    //  Serial.print(" ");
    //}
    /*! Only through 3 times checks, the received data will be confirmed as accurate*/
    if(checkDCS(16) == 1 && receiveACK[12] == 0x41 && receiveACK[13] == 0x00)   
        return true;
    else
        return false; 
}
struct card DFRobot_PN532::getInformation(){
    struct card card;
    uint8_t cmdnfcUid[11];
    cmdnfcUid[0] = COMMAND_INLISTPASSIVETARGET;
    cmdnfcUid[1] = 1;                              // The quantity number of the maxium card that can be detected in every research
    cmdnfcUid[2] = MIFARE_ISO14443A;
    writeCommand(cmdnfcUid,3);
    readAck(28);
    for(int i = 0; i < receiveACK[18]; i++)
        nfcUid[i] = receiveACK[i + 19];
    
    /*for(int i= 0 ; i<32 ;i++){
        Serial.print(receiveACK[i],HEX);
        Serial.print(" ");
    }*/
    //Serial.println(" ");
    card.AQTA[0] = receiveACK[15];
    card.AQTA[1] = receiveACK[16];
    card.SAK = receiveACK[17];
    memcpy(card.uid,receiveACK+19,receiveACK[18]);
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x04){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",20);
        memcpy(card.cardType,"MIFARE Classic 1k",17);
        card.uidlenght = 4;
        memcpy(card.Manufacturer,"NXP",3);
        card.size = 1024;
        card.usersize = 752;
        card.blockSize = 16;
        card.blockNumber = 64;
        card.sectorSize = 16;
    }
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(230) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"NTAG 216",8);
        memcpy(card.Manufacturer,"NXP",4);
        card.uidlenght = 7;
        card.size = 924;
        card.usersize = 888;
        card.blockSize = 4;
        card.blockNumber = 231;
        card.sectorSize = 1;
    }
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(230) != 1 && getUltraversion(134) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"NTAG 215",8);
        memcpy(card.Manufacturer,"NXP",4);
        card.uidlenght = 7;
        card.size = 540;
        card.usersize = 504;
        card.blockSize = 4;
        card.blockNumber = 135;
        card.sectorSize = 1;
    }
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(134) != 1 && getUltraversion(44) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"NTAG 213",8);
        memcpy(card.Manufacturer,"NXP",4);
        card.usersize = 144;
        card.uidlenght = 7;
        card.size = 180;
        card.blockSize = 4;
        card.blockNumber = 45;
        card.sectorSize = 1;
    }
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(44) != 1 && getUltraversion(40) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"Ultralight",10);
        memcpy(card.Manufacturer,"NXP",4);
        card.usersize = 128;
        card.uidlenght = 7;
        card.size = 164;
        card.blockSize = 4;
        card.blockNumber = 41;
        card.sectorSize = 1;
    }
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(40) != 1 && getUltraversion(19) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"Ultralight",10);
        memcpy(card.Manufacturer,"NXP",4);
        card.usersize = 48;
        card.uidlenght = 7;
        card.size = 80;
        card.blockSize = 4;
        card.blockNumber = 20;
        card.sectorSize = 1;
    }
    
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x44 && getUltraversion(19) != 1 && getUltraversion(14) == 1){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",25);
        memcpy(card.cardType,"Ultralight",10);
        memcpy(card.Manufacturer,"NXP",4);
        card.uidlenght = 7;
        card.size = 448;
        card.usersize = 320;
        card.blockSize = 4;
        card.blockNumber = 41;
        card.sectorSize = 1;
    }
    
    
    
    if(card.AQTA[0] == 0x00 && card.AQTA[1] ==0x02){
        memcpy(card.RFTechnology,"ISO/IEC1443-3,Type A",20);
        memcpy(card.cardType,"MIFARE Classic 4k",17);
        memcpy(card.Manufacturer,"NXP",3);
        card.uidlenght = 7;
        card.size = 4096;
        card.usersize = 3440;
        card.blockSize = 16;
        card.blockNumber = 256;
        card.sectorSize = 39;
    }
    
    
    return card;    
}
bool DFRobot_PN532::checkDCS(int x)  
{
    if(!this->nfcEnable)
        return 0;
    unsigned char sum=0,dcs=0;
    /*! Calculate the DSC value and compare it with the DSC from Ack*/
    for(int i = 6;i < x - 2;i++)
    {
        sum += this->receiveACK[i];
    }
    dcs = 0xff - sum&0xff;
    if(dcs==this->receiveACK[x - 2])
        return 1;
    else
        return 0;
}
bool DFRobot_PN532::writeData(int block, uint8_t data[])
{   if(block < 128 && ( (block + 1)%4 == 0 || block ==0 ))
        return false;
    if((block >127 && block <256) && ((block + 1)%16 == 0))
        return false;
    if(block > 255)
        return false;
    if(!this->nfcEnable)
        return false;
    if(!this->scan())
        return false;
    if(!this->passWordCheck(block,nfcUid,nfcPassword))
        return false;
    unsigned char cmdWrite[20];
        cmdWrite[0] = COMMAND_INDATAEXCHANGE;
        cmdWrite[1] = 1;                                     /* Card number */
        cmdWrite[2] = CARD_CMD_WRITEINGTOMIFARECLASSIC;       /* MifareClassic Write command = 0xA0 */
        cmdWrite[3] = block;
    unsigned char sum=0,count=0;
    for(int i = 4;i < 20;i++) cmdWrite[i]=data[i - 4];// Data to be written
    this->writeCommand(cmdWrite,20);
    this->readAck(16);
    return true;
}
/*!
   It takes three steps to write a piece of data to a card
   1.Read out all the data for one block(every block have 16 data.).
   2.Change one data from sixteen data.
   3.Write 16 data changes to the card.
   so ...
*/
void DFRobot_PN532::writeData(int block, uint8_t index, uint8_t data)
{
    if(!this->nfcEnable)
        return;
    index = max(min(index,16),1);
    this->readData(block);
    this->blockData[index - 1] = data;
    this->writeData(block, this->blockData);
}
uint8_t DFRobot_PN532::readData(int block, uint8_t offset)
{
    if(!this->nfcEnable)
        return -1;
    String error = this->readData(block);
    if(error == "no card!" || error == "read error!" || error =="unknown error!"
                        || error == "read timeout!" || error == "wake up error!")
        return -1;
    return this->blockData[offset - 1];
}

bool DFRobot_PN532::scan(String nfcUid)
{
    if(!this->nfcEnable)
        return false;
    if(this->scan())
    {
        String nfcUidSrt = "";
        for(int i=0; i<4; i++)
        {
            if(this->nfcUid[i] <= 0x0f){
                nfcUidSrt += "0";
                nfcUidSrt += String(this->nfcUid[i],HEX);
            }
            else
                nfcUidSrt += String(this->nfcUid[i],HEX);
        }
        if(nfcUid == nfcUidSrt)
            return true;
    }
    return false;
}

bool DFRobot_PN532::scan()
{   if(!this->nfcEnable)
        return false;
    uint8_t cmdnfcUid[11];
    cmdnfcUid[0] = COMMAND_INLISTPASSIVETARGET;
    cmdnfcUid[1] = 1;                              // The quantity number of the maxium card that can be detected in every research
    cmdnfcUid[2] = MIFARE_ISO14443A;
    writeCommand(cmdnfcUid,3);
    if(!readAck(25))
        return false;
    for(int i = 0; i < 4; i++)
        nfcUid[i] = receiveACK[i + 19];
    /*for(int i= 0 ; i<32 ;i++){
        Serial.print(receiveACK[i]);
        Serial.print(" ");
    }*/
    if(receiveACK[13]!=1)
        return false;
    return true;
}

String DFRobot_PN532::readUid()
{   if(!this->nfcEnable)
        return "wake up error!";
    if(!scan())
        return "no card!";
    String nfcUidSrt = "";
    for(int i = 0; i < 4; i++)
    {
        if(nfcUid[i] <= 0x0f){
            nfcUidSrt += "0";
            nfcUidSrt += String(nfcUid[i],HEX);
        }
        else
            nfcUidSrt += String(nfcUid[i],HEX);
    }
    return nfcUidSrt;
}
/*!
 There is 1kb of storage space in the card,this space is divided into 16 
 areas and every area has four blocks of data,One of the four blocks stores 
 the password that cannot be changed.

*/


/*!
 The 1024 × 8 bit EEPROM memory is organized in 16 sectors of 4 blocks. One block
contains 16 bytes.
     Sector  0       block 0           (Manufacturer   block)              Manufacturer  Data  
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


uint8_t DFRobot_PN532::readData(uint8_t *buffer,uint8_t block){
    if(!this->nfcEnable)
        return -1;
    String error = this->readData(block);
    if(error == "no card!" || error == "read error!" || error =="unknown error!"
                        || error == "read timeout!" || error == "wake up error!" ||error =="flase")
        return -1;
    memcpy(buffer,blockData,16);
    return  1;
    
}
String DFRobot_PN532::readData(int page) {
    if (page > 255)
        return "flase";
    if(!this->nfcEnable)
        return "wake up error!";
    if(!scan())
        return "no card!";
    if(!passWordCheck(page,nfcUid,nfcPassword))
        return "read error!";
    unsigned char cmdRead[4];
    unsigned char sum = 0,count = 0;
        cmdRead[0] = COMMAND_INDATAEXCHANGE;
        cmdRead[1] = 1;                   /* Card number */
        cmdRead[2] = CARD_CMD_READING;     /* Mifare Read command = 0x30 */
        cmdRead[3] = page; 
    
    writeCommand(cmdRead,4);
    if(!readAck(32))
        return "read timeout!";
    String dataSrt = "";
    if(checkDCS(32) == 1 && receiveACK[12] == 0x41 && receiveACK[13] == 0x00){
        for(int i = 0; i<16; i++)
        {
            blockData[i] = receiveACK[i + 14];
            if(receiveACK[i+14]<=0x0f){
                dataSrt += "0";
                dataSrt += String(receiveACK[i + 14],HEX);
            }
            else
                dataSrt += String(receiveACK[i + 14],HEX);
            if(i<15) dataSrt += " ";
        }
    }
    return dataSrt;
}
/*
    Send commands to the chip through the iic ports*/

void DFRobot_PN532_IIC::writeCommand(uint8_t* cmd, uint8_t cmdlen) {     
    uint8_t checksum;
    cmdlen++;
    delay(2);     // Delay for random time to wake up NFC module
    // I2C START
    Wire.beginTransmission(I2C_ADDRESS);
    checksum = PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2;
    Wire.write(PN532_PREAMBLE);
    Wire.write(PN532_STARTCODE1);
    Wire.write(PN532_STARTCODE2);
    Wire.write(cmdlen);
    Wire.write(~cmdlen + 1);
    Wire.write(HOSTTOPN532);
    checksum += HOSTTOPN532;
    for (uint8_t i = 0; i < cmdlen - 1; i++) {
      Wire.write(cmd[i]);
      checksum += cmd[i];
    }
    Wire.write((byte)~checksum);
    Wire.write((byte)PN532_POSTAMBLE);
    Wire.endTransmission();
}

bool DFRobot_PN532_IIC::readAck(int x,long timeout ) {
    uint8_t pn532ack[6];
    pn532ack[0] = 0x00;
    pn532ack[1] = 0x00;
    pn532ack[2] = 0xFF;
    pn532ack[3] = 0x00;
    pn532ack[4] = 0xFF;
    pn532ack[5] = 0x00;
    if(_mode == 1){
    if(!waitRemind())
        return false;
    Wire.requestFrom(I2C_ADDRESS,8);
    Wire.read();
    for(int i = 0; i < 6; i++){
        delay(1);
        receiveACK[i]= Wire.read();
    }
    if(!waitRemind() ) return false;
    
    Wire.requestFrom(I2C_ADDRESS,x-4);
    Wire.read();
    for(int i = 0; i < x - 6; i++){
        delay(1);
        receiveACK[6 + i] = Wire.read();
    }
    
    }
    else if(_mode == 0){
    delay(30);
    Wire.requestFrom(I2C_ADDRESS,8);
    Wire.read();
    for(int i = 0; i < 6; i++){
        delay(1);
        receiveACK[i]= Wire.read();
    }
    
    delay(30);
    Wire.requestFrom(I2C_ADDRESS,x-4);
    Wire.read();
    for(int i = 0; i < x - 6; i++){
        delay(1);
        receiveACK[6 + i] = Wire.read();
    }
    }
    if(strncmp((char *)pn532ack,(char *)receiveACK, 6)!=0){
        return false ;
    }
    return true;
}
DFRobot_PN532_IIC::DFRobot_PN532_IIC(uint8_t irq,uint8_t mode){
    
    _irq = irq;
    pinMode(_irq, INPUT);
    _mode = mode;
}
bool DFRobot_PN532_IIC::waitRemind(){
    uint16_t timeout = 1000;
    uint16_t timer = 0;
     //digitalRead(_irq)
    //return ret == 0;
    //bool b = digitalRead(_irq);
    //Serial.println(b);
    while(digitalRead(_irq)!=0){
        //Serial.print(digitalRead(_irq));
    if (timeout != 0) {
      timer += 10;
      if (timer > timeout) {
        return false;
      }
    }
    delay(10);
    }
    //Serial.print(digitalRead(_irq));
    return true;
}
bool DFRobot_PN532_IIC::begin(void) {   //nfc Module initialization  
    this->nfcPassword[0] = 0xff;
    this->nfcPassword[1] = 0xff;
    this->nfcPassword[2] = 0xff;
    this->nfcPassword[3] = 0xff;
    this->nfcPassword[4] = 0xff;
    this->nfcPassword[5] = 0xff;
    unsigned char cmdWrite[4];
    cmdWrite[0] = COMMAND_SAMCONFIGURATION;
    cmdWrite[1] = 0x01; // normal mode;
    cmdWrite[2] = 0x14; // timeout 50ms * 20 = 1 second
    cmdWrite[3] = 0x01; // use IRQ pin!
    Wire.begin();
    nfcEnable = true;
    writeCommand(cmdWrite,4);
    delay(10);
    
    if(readAck(14)!= 1){
        
        return false;
    }
    return  ( receiveACK[12] == 0x15);
}
#ifdef ESP_PLATFORM
bool DFRobot_PN532_UART::begin(HardwareSerial *serial, int rx, int tx)
{   this->uartTimeout = 1000;
    this->_serial = serial;
    this->_serial->begin(115200,rx,tx);

#else
bool DFRobot_PN532_UART::begin(HardwareSerial *serial)
{   this->uartTimeout = 1000;
    this->_serial = serial;
    this->_serial->begin(115200);
#endif
    this->nfcPassword[0] = 0xff;
    this->nfcPassword[1] = 0xff;
    this->nfcPassword[2] = 0xff;
    this->nfcPassword[3] = 0xff;
    this->nfcPassword[4] = 0xff;
    this->nfcPassword[5] = 0xff;
    memset(this->receiveACK,0,35);
    memset(this->blockData,0,16);
    memset(this->nfcUid,0,4);
    unsigned char wake[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd4, 0x14, 0x01, 0x17, 0x00};//Wake up NFC module
    for(int i = 0; i < 24; i++)
    {
        this->_serial->write(wake[i]);
#if defined(ARDUINO) && ARDUINO >= 100
#ifndef ESP_PLATFORM
        this->_serial->flush();// Complete the transmission of outgoing serial data
#endif
#endif
    }
    if(!this->readAck(15)){
    this->nfcEnable = false;
    return false;
    }
    this->nfcEnable = true;
    return  (receiveACK[12] == 0x15);;
}
bool DFRobot_PN532_UART::readAck(int x, long timeout) //Read the data from the serial port
{   uint8_t pn532ack[6];
    pn532ack[0] = 0x00;
    pn532ack[1] = 0x00;
    pn532ack[2] = 0xFF;
    pn532ack[3] = 0x00;
    pn532ack[4] = 0xFF;
    pn532ack[5] = 0x00;
    //unsigned char i = x;
    long _timeout = millis();
    delay(100);
        if(this->_serial->available()){
            
        for(int i = 0; i<=x ;i++){
            receiveACK[i] = _serial->read();
        }
        }
        //for(int i= 0 ; i<32 ;i++){
        //Serial.print(receiveACK[i],HEX);
        //Serial.print(" ");
    //}
        if(strncmp((char *)pn532ack,(char *)receiveACK, 6)!=0){
        return false ;
    }
    return true;
            
}
/*Send commands to the chip through serial ports*/
void DFRobot_PN532_UART::writeCommand(uint8_t *command_data, uint8_t bytes)
{   
    if(this->_serial == NULL)
        return;
    char residual;
    long timeout = millis();
    while(this->_serial->available()){
        residual = this->_serial->read();
        if(millis() - timeout >= this->uartTimeout && this->_serial->available())
            return ;
    }
    uint8_t checksum;
    bytes++;
    delay(2);     // Delay for random time to wake up NFC module
    // I2C START
    checksum = PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2;
    this->_serial->write((byte)PN532_PREAMBLE);
    this->_serial->write((byte)PN532_STARTCODE1);
    this->_serial->write((byte)PN532_STARTCODE2);

    this->_serial->write(bytes);
    this->_serial->write(~bytes + 1);

    this->_serial->write(HOSTTOPN532);
    checksum += HOSTTOPN532;

    for (uint8_t i = 0; i < bytes-1; i++) {
       this->_serial->write(command_data[i]);
       checksum += command_data[i];
    }

    this->_serial->write((byte)~checksum);
    this->_serial->write((byte)PN532_POSTAMBLE);
#if defined(ARDUINO) && ARDUINO >= 100
#ifndef ESP_PLATFORM
    this->_serial->flush();/* Complete the transmission of outgoing serial data*/
#endif
#endif
    
}