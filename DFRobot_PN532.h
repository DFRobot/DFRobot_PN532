/*
 MIT License
 Copyright (C) <2019> <@DFRobot Frank>
¡¡Permission is hereby granted, free of charge, to any person obtaining a copy of this
¡¡software and associated documentation files (the "Software"), to deal in the Software
¡¡without restriction, including without limitation the rights to use, copy, modify,
¡¡merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
¡¡permit persons to whom the Software is furnished to do so.
¡¡The above copyright notice and this permission notice shall be included in all copies or
¡¡substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DFROBOT_PN532_H
#define DFROBOT_PN532_H

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#define PN532_PREAMBLE                      (0x00)
#define PN532_STARTCODE1                    (0x00)
#define PN532_STARTCODE2                    (0xFF)
#define PN532_POSTAMBLE                     (0x00)
#define HOSTTOPN532                         (0xD4)
#define PN532TOHOST                         (0xD5)
// PN532 Commands
#define COMMAND_SAMCONFIGURATION            (0x14)
#define COMMAND_INLISTPASSIVETARGET         (0x4A)
#define COMMAND_INDATAEXCHANGE              (0x40)
#define I2C_ADDRESS                    (0x48 >> 1)
#define MIFARE_ISO14443A                    (0x00)
// CARD Commands
#define CARD_CMD_READING                     (0x30)
#define CARD_CMD_WRITEINGTOMIFARECLASSIC     (0xA0)
#define CARD_CMD_WRITEINGTONTGE              (0xA2)
#define CARD_CMD_WRITEINGTOULTRALIGHT        (0xA2)
#define CARD_CMD_AUTHENTICATION_A            (0x60)
#define CARD_CMD_AUTHENTICATION_B            (0x61)


 struct card{
    uint8_t uidlenght;
    int size;
    int usersize;
    int blockSize;
    int blockNumber;
    uint8_t sectorSize;
    uint8_t SAK;
    uint8_t AQTA[2];
    char RFTechnology[25]={0};
    char Manufacturer[6]={0};
    uint8_t uid[7];
    char cardType[30]={0};
};

class DFRobot_PN532
{  
   public: 
    

        /*!
         * @brief Read a block from a MIFARE Classic NFC smart card/tag (16 bytes each block).
         * @param The buffer of the read data.
         * @param The number of the block to read from.
         * @return Status code. 
         */   
        uint8_t readData(uint8_t *buffer,uint8_t block);

        /*!
         * @brief Read a byte from a specified block of a MIFARE Classic NFC smart card/tag.
         * @param The number of the block to read from.
         * @param The offset of the block. 
         * @return A byte read from the card. 
         */  
        uint8_t readData(int block, uint8_t offset);

        /*!
         * @brief Write a byte to a MIFARE Classic NFC smart card/tag.
         * @param The number of pages you want to writes the data.
         * @param The offset of the data. 
         * @param The byte to be written.
         */  
        void writeData (int block, uint8_t num ,uint8_t data);

        /*!
         * @brief Scan to determine whether there is a NFC smart card/tag. 
         * @return True means find out a MIFARE Classic card.
         */ 
        bool  scan(void);

        /*!
         * @brief Scan to determine whether there is a NFC smart card/tag with the specified UID.
         * @param UID of the NFC card.
         * @return True means success.
         */   
        bool  scan(String nfcuid);

        /*!
         * @brief Obtain the UID of the card .
         * @return UID of the card.
         */  
        String  readUid();

        /*!
         * @brief Write a block to a MIFARE Classic NFC smart card/tag..
         * @param The number of the block to write to.
         * @param The buffer of the data to be written. 
         */  
        bool  writeData(int block, uint8_t data[]);

        /*!
         * @brief Read a page from a MIFARE Ultralight NFC smart card/tag.
         * @param The buffer of the read data. 
         * @param The number of the block to read from.
         * @return Error or correct message code
         */
        uint8_t readUltralight(uint8_t *buffer,uint8_t block);

        /*!
         * @brief Write a page to a MIFARE Ultralight NFC smart card/tag.
         * @param The number of the block to read from.
         * @param The buffer of the read data. 
         * @return Error or correct message code
         */
        bool  writeUltralight(int block, uint8_t data[]);

        /*!
         * @brief Read a page from a NTAG.
         * @param The buffer of the read data.
         * @param The number of the block to read from.
         * @return error or correct message code.
         */
        uint8_t readNTAG(uint8_t *buffer,uint8_t block);

        /*!
         * @brief Write a page to a NTAG.
         * @param The number of the block to read from.
         * @param The buffer of the read data. 
         * @return Error or correct message code
         */
        bool  writeNTAG(int block, uint8_t data[]);

        /*!
         * @brief Read the basic information of a NFC smart card/tag. 
         * @return Info. of the card.
         */
struct card getInformation();
     
     /*!buffer for  data from serial port*/
     uint8_t receiveACK[35];    
      
      /*!the default passWord is 0xff 0xff 0xff 0xff 0xff 0xff.*/
     uint8_t nfcPassword[6]; 
     
     /*!buffer for card's id*/
     uint8_t nfcUid[4]; 
     
     /*!buffer  for  data is written*/
     uint8_t blockData[16];
     bool nfcEnable;
     long uartTimeout; 
     uint8_t _irq;
     uint8_t _mode;
     
  private:
      
    /*!
     * @brief read a page of data from the target card(there are 16 bytes of data per page).
     * @param The number of pages that you want to read data.
     * @return a page of data 
     */   
     String readData(int page);
    
    /*!
     * @brief Send a command to the pn532 chip.
     * @param *command_data: the address of the array containing the command.
     * @param bytes:the length of the arry. 
     */  
      virtual void writeCommand(uint8_t *command_data, uint8_t bytes)=0;
    
    /*!
     * @brief Read the reply data from pn532
     * @param x: the length of the data.
     * @param timeout: the maximum time value to wait for a response. 
     * @return True means success.
     */ 
      bool virtual readAck(int x,long timeout = 1000)=0;
    
    /*!
     * @brief Check the password
     * @param blockNumber: the number of pages you want to operate.
     * @param nfcuid[]: the card's id. 
     * @param keyData[]: keywords.
     * @return True means success.
     */ 
      bool  passWordCheck (int blockNumber,uint8_t nfcuid[],  uint8_t keyData[]);
    
    /*!
     * @brief verify data received from pn532 chip is correct.
     * @param x: the length of the data.
     * @return True means correct.
     */   
      bool  checkDCS(int x);
     
     /*!
     * @brief  Get the version of the card.
     * @param block: the number of pages you want to verify.
     * @return .
     */ 
      uint8_t getUltraversion(uint8_t block);
      
};
class DFRobot_PN532_IIC : public DFRobot_PN532
{
public:
    DFRobot_PN532_IIC(uint8_t irq,uint8_t mode);
    bool begin(void);
    void writeCommand(uint8_t* cmd, uint8_t cmdlen);
    bool readAck(int x,long timeout = 1000);     
private:
    bool waitRemind();
};

class DFRobot_PN532_UART:public DFRobot_PN532
{   
public:
#ifdef ESP_PLATFORM
    bool begin(HardwareSerial *serial, int rx, int tx);
#else
    bool begin(HardwareSerial *serial);
#endif
private:
    HardwareSerial *_serial;                
    void writeCommand(uint8_t *command_data, uint8_t bytes);
    bool readAck(int x, long timeout = 1000);        
};
#endif