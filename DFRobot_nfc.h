/*
===============================================
pn532 accelerometer/gyroscope library for Intel(R) Curie(TM) devices.
Copyright (c) 2015 Intel Corporation.  All rights reserved.
===============================================
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef DFROBOT_NFC_H
#define DFROBOT_NFC_H

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
// Mifare Commands
#define CARD_CMD_READING                     (0x30)
#define CARD_CMD_WRITEING                    (0xA0)
#define CARD_CMD_AUTHENTICATION_A            (0x60)
#define CARD_CMD_AUTHENTICATION_B            (0x61)




class DFRobot_PN532
{  
   public: 
    /*!
     * @brief read a page of data from the target card(there are 16 bytes of data per page).
     * @param The number of pages that you want to read data.
     * @return a page of data 
     */   
     String readData(int page);
    /*!
     * @brief read a data from the  page which you want.
     * @param page: the number of pages you want to read data.
     * @param num: the offset of the data that you want. 
     * @return a data from the card. 
     */  
     int  readData(int page, uint8_t num);
    /*!
     * @brief write a data to card .
     * @param page: the number of pages you want to writes the data.
     * @param num: the offset of the data. 
     * @param data: the data content.
     */  
     void  writeData (int page, uint8_t num ,uint8_t data);
    /*!
     * @brief find out if there is a card. 
     * @return True means find out a card.
     */ 
     bool  scan(void);
    /*!
     * @brief find out if there is a specific card.
     * @param a string of card Numbers.
     * @return True means success.
     */   
     bool  scan(String nfcuid);
    /*!
     * @brief get the id of the card .
     * @return  the card's id.
     */  
     String  readUid();
     /*!
     * @brief write sixteen data on the card.
     * @param block: the number of pages you want to write data on.
     * @param data[]: the name of arry name which has sixteen data . 
     */  
     void  writeData(int block, uint8_t data[]);
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
  private:
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
      ;
};
class DFRobot_PN532_IIC : public DFRobot_PN532
{
public:
    bool begin(void);
    void writeCommand(uint8_t* cmd, uint8_t cmdlen);
    bool readAck(int x,long timeout = 1000);     
private:
};

class DFRobot_PN532_UART:public DFRobot_PN532
{   
public:
#ifdef ESP_PLATFORM
    void begin(HardwareSerial *serial, int rx, int tx);
#else
    void begin(HardwareSerial *serial);
#endif
private:
    HardwareSerial *_serial;                
    void writeCommand(uint8_t *command_data, uint8_t bytes);
    bool readAck(int x, long timeout = 1000);        
};
#endif