# DFRobot_PN532
The PN532 is a highly integrated transmission module for contactless communication at 13.56 MHz including microcontroller functionality based on an 80C51 core with 40 Kbytes of ROM and 1 Kbytes of RAM. 
The PN532 combines a modulation and demodulation concept completely integrated for different kinds of contactless communication methods and protocols at 13.56 MHz with an easy-to-use firmware for the different supported modes and the required host controller interfaces. 
This example is for pn532 chip reads and writes CARDS.
## DFRobot_PN532 Library for Arduino
---------------------------------------------------------

Provide an Arduino library to control the nfc module to get data from card and writes data  to card .

## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Support](#support)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++	

#include <DFRobot_PN532.h>


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
 * @brief Read the basic information of a NFC smart card/tag. （All the supported card is listed below）
 * @return Info. of the card.
 */
struct card getInformation();
	 
```
## Support
TYPE                | read and write   | Can be accurately identified？ | Remarks
------------------- | :--------------: | :----------------------------: |-----
Mifareclassic S50   |      √           |      √                         | 
Mifareclassic S70   |      √           |      √                         |                         
NTAG 213            |      √           |       √                         |        
NTAG 215            |      √           |       √                         |    
NTAG 216            |      √           |       √                         |          
Ultralight          |      √           |       √                         |           


## Compatibility

MCU                | iic          | uart         | Remarks
------------------ | :----------: | :----------: |-----
ESP32              |      √       |      √       |             
micro:bit          |      √       |              |              
Arduino uno        |      √       |              |           
Arduino mega 2560  |      √       |      √       |

## History

- data 2019-6-10
- version V0.1


## Credits

Written by fengli(1074485750@qq.com), 2017. (Welcome to our [website](https://www.dfrobot.com/))





