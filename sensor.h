/*-----------------------------------
* File name: sensor.h
* Author: Sameid Usmani
* Description: A short description of the module.
*-----------------------------------*/

#include <I2CMultimaster.h>

#define TPA81_WRITE 0xD0
#define TPA81_READ 0xD1

#define MASTER 0xA0

#define AMBIENT 0x01
#define PIXEL_1 0x02
#define PIXEL_2 0x03
#define PIXEL_3 0x04
#define PIXEL_4 0x05
#define PIXEL_5 0x06
#define PIXEL_6 0x07
#define PIXEL_7 0x08
#define PIXEL_8 0x09


uint8_t getAmbientTemperature(void);
uint8_t getDataFromRegister (uint8_t);
