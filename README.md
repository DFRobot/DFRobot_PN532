# pn532
The PN532 is a highly integrated transmission module for contactless communication at 13.56 MHz including microcontroller functionality based on an 80C51 core with 40 Kbytes of ROM and 1 Kbytes of RAM. 
The PN532 combines a modulation and demodulation concept completely integrated for different kinds of contactless communication methods and protocols at 13.56 MHz with an easy-to-use firmware for the different supported modes and the required host controller interfaces. 
This example is for pn532 chip reads and writes CARDS.
## DFRobot_nfc Library for Arduino
---------------------------------------------------------

Provide an Arduino library to control the nfc module to get data from card and writes data  to card .

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

Provide an Arduino library to control the nfc module to get data from card and writes data  to card .

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++	

#include <DFRobot_nfc.h>

/*
 * @brief read a page of data from the target card(there are 16 bytes of data per page).
 * @param the number of pages you want to read data.
 * @return a page of data 
 */   
String readData(int page);
	 
/*
 * @brief read a data from the page that you want.
 * @param page: the number of pages you want to read data.
 * @param num: the offset of the data that you want. 
 * @return a data from the card. 
 */
int  readData(int page, uint8_t num);
	 
/*
 * @brief write a data to card .
 * @param page: the number of pages you want to writes the data.
 * @param num: the offset of the data. 
 * @param data:the data content.
 */  
void  writeData (int page, uint8_t num ,uint8_t data);
	 
/*
 * @brief find out if there is a card. 
 * @return True means find out a card.
 */ 
bool  scan(void);
	 
/*
 * @brief find out if there is a specific card.
 * @param a string of card Numbers.
 * @return True means success.
 */   
bool  scan(String uid);
	 
/*
 * @brief get the id of the card.
 * @return  the card's id.
 */  
String  readUid();
	 
/*
 * @brief write sixteen data on the card.
 * @param block: the number of pages you want to write data.
 * @param data[]: the name of arry name which has sixteen data. 
 */  
void  writeData(int block, uint8_t data[]);
	 
```

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





