/*!
 *@file DFRobot_PN532.h
 *@brief Define the basic structure of class DFRobot_PN532, the implementation of basic methods.
 *@details this is a module that reads data from NFC tags
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2019-10-15
 *@url https://github.com/DFRobot/DFRobot_PN532
*/

#ifndef DFROBOT_PN532_H
#define DFROBOT_PN532_H

#include <Wire.h>
#include "Arduino.h"

#define PN532_PACKBUFFSIZ                   (64  )//The size of the packet buffer
#define PN532_PREAMBLE                      (0x00)
#define PN532_STARTCODE1                    (0x00)
#define PN532_STARTCODE2                    (0xFF)
#define PN532_POSTAMBLE                     (0x00)
#define HOSTTOPN532                         (0xD4)
#define PN532TOHOST                         (0xD5)
// PN532 Commands
#define COMMAND_SAMCONFIGURATION            (0x14)//SAM Configuration Commands
#define COMMAND_INLISTPASSIVETARGET         (0x4A)
#define COMMAND_INDATAEXCHANGE              (0x40)
#define I2C_ADDRESS                    (0x48 >> 1)//Device address
#define MIFARE_ISO14443A                    (0x00)
// CARD Commands
#define CARD_CMD_READING                     (0x30)//Command to read data
#define CARD_CMD_WRITEINGTOMIFARECLASSIC     (0xA0)//Command to write a card of type MifareClassic
#define CARD_CMD_WRITEINGTONTGE              (0xA2)//Command for writing NTGE cards
#define CARD_CMD_WRITEINGTOULTRALIGHT        (0xA2)// Command for writing ultralight cards
#define CARD_CMD_AUTHENTICATION_A            (0x60)//The command to authenticate with the A-block password
#define CARD_CMD_AUTHENTICATION_B            (0x61)//The command to authenticate with the B-block password




class DFRobot_PN532
{  
public: 
  typedef struct{
      uint8_t uidlenght;/**<The length of the uid*/
      int size;            
      int usersize;
      int blockSize;    /**<The size of the block*/
      int blockNumber;  /**<The serial number of block*/
      uint8_t sectorSize; /**<Sector size*/
      uint8_t SAK;  /**<SAK code*/
      uint8_t AQTA[2];  /**<AQTA*/
      char RFTechnology[25]={0};/**<Radio frequency information*/
      char Manufacturer[6]={0};/**<Vendor string*/
      uint8_t uid[7];    /**<Uid content*/
      char cardType[30]={0};/**<The chip type*/
  }sCard_t;
public: 
   /*!
    * @fn readData
    * @brief Read a block from a MIFARE Classic NFC smart card/tag (16 bytes each block).
    * @param buffer The buffer of the read data.
    * @param block The number of the block to read from.
    * @return Status code. 
    * @retval 1 successfully read data
    * @retval -1 Failed to read data
    */   
   uint8_t readData(uint8_t *buffer,uint8_t block);

   /*!
    * @fn readData
    * @brief Read a byte from a specified block of a MIFARE Classic NFC smart card/tag.
    * @param block The number of the block to read from.
    * @param offset The offset of the block. 
    * @return A byte read from the card. 
    */  
   uint8_t readData(int block, uint8_t offset);

   /*!
    * @fn writeData
    * @brief Write a byte to a MIFARE Classic NFC smart card/tag.
    * @param block The number of pages you want to writes the data.
    * @param num The offset of the data. 
    * @param data The byte to be written.
    */  
   void writeData (int block, uint8_t num ,uint8_t data);

   /*!
    * @fn scan
    * @brief Scan to determine whether there is a NFC smart card/tag. 
    * @return Boolean type, the result of operation
    * @retval true means find out a MIFARE Classic card.
    * @retval false no card
    */ 
   bool  scan(void);

   /*!
    * @fn scan
    * @brief Scan to determine whether there is a NFC smart card/tag with the specified UID.
    * @param nfcuid UID of the NFC card.
    * @return Boolean type, the result of operation
    * @retval true Finds a card with a specific UID
    * @retval false The card with a specific UID was not found
    */   
   bool  scan(String nfcuid);

   /*!
    * @fn readUid
    * @brief Obtain the UID of the card .
    * @return UID of the card.
    */  
   String  readUid();

   /*!
    * @fn writeData
    * @brief Write a block to a MIFARE Classic NFC smart card/tag..
    * @param block The number of the block to write to.
    * @param data The buffer of the data to be written. 
    */  
   bool  writeData(int block, uint8_t data[]);

   /*!
    * @fn readUltralight
    * @brief Read a page from a MIFARE Ultralight NFC smart card/tag.
    * @param buffer The buffer of the read data. 
    * @param block The number of the block to read from.
    * @return Error or correct message code
    */
   uint8_t readUltralight(uint8_t *buffer,uint8_t block);

   /*!
    * @fn writeUltralight
    * @brief Write a page to a MIFARE Ultralight NFC smart card/tag.
    * @param block The number of the block to read from.
    * @param data The buffer of the read data. 
    * @return Boolean type, the result of operation
    * @retval true Write success
    * @retval false Write failed
    */
   bool  writeUltralight(int block, uint8_t data[]);

   /*!
    * @fn readNTAG
    * @brief Read a page from a NTAG.
    * @param buffer The buffer of the read data.
    * @param block The number of the block to read from.
    * @return Status code. 
    * @retval 1 successfully read data
    * @retval -1 Failed to read data
    */
   uint8_t readNTAG(uint8_t *buffer,uint8_t block);

   /*!
    * @fn writeNTAG
    * @brief Write a page to a NTAG.
    * @param block The number of the block to read from.
    * @param data The buffer of the read data. 
    * @return Boolean type, the result of operation
    * @retval true Write success
    * @retval false Write failed
    */
   bool  writeNTAG(int block, uint8_t data[]);

   /*!
    * @fn readData
    * @brief Read the basic information of a NFC smart card/tag. 
    * @return Info. of the sCard_t.
    */
   sCard_t getInformation();
     

   uint8_t receiveACK[35];    
   uint8_t nfcPassword[6]; 
   uint8_t nfcUid[4]; 
   uint8_t blockData[16];
   bool nfcEnable;
   long uartTimeout; 
   uint8_t _irq;
   uint8_t _mode;
     
private:
       
   String readData(int page);
   virtual void writeCommand(uint8_t *command_data, uint8_t bytes)=0;
   bool virtual readAck(int x,long timeout = 1000)=0;
   bool  passWordCheck (int blockNumber,uint8_t nfcuid[],  uint8_t keyData[]);
   bool  checkDCS(int x);
   uint8_t getUltraversion(uint8_t block);
      
};
class DFRobot_PN532_IIC : public DFRobot_PN532
{
public:

  /*!
   * @fn DFRobot_PN532_IIC
   * @brief constructor
   * @param irq interrupt pin
   * @param mode Data read mode (interrupt/polling)
   */
   DFRobot_PN532_IIC(uint8_t irq,uint8_t mode);
   
  /*!
   * @fn begin
   * @brief Initializes the NFC chip
   * @return Boolean type, the result of operation
   * @retval true Initialization succeeded
   * @retval false Initialization failed
   */
   bool begin(void);
    
        
private:
    void writeCommand(uint8_t* cmd, uint8_t cmdlen);
    bool readAck(int x,long timeout = 1000); 
    bool waitRemind();
};

class DFRobot_PN532_UART:public DFRobot_PN532
{   
public:
#ifdef ESP_PLATFORM
   /*!
    * @fn begin
    * @brief Initializes the NFC chip
    * @param Serial Indicates a hard serial port
    * @param rx custom Rx pins
    * @param tx Custom TX pin
    * @return Boolean type, the result of operation
    * @retval true Initialization succeeded
    * @retval false Initialization failed
    */
    bool begin(HardwareSerial *serial, int rx, int tx);
#else
   /*!
    * @fn begin
    * @brief Initializes the NFC chip
    * @param Serial Indicates a hard serial port
    * @return Boolean type, the result of operation
    * @retval true Initialization succeeded
    * @retval false Initialization failed
    */  
    bool begin(HardwareSerial *serial);
#endif
private:
    HardwareSerial *_serial;                
    void writeCommand(uint8_t *command_data, uint8_t bytes);
    bool readAck(int x, long timeout = 1000);        
};
#endif