/*-----------------------------------
* File name: sensor.h
* Author: Sameid Usmani
* Description: A short description of the module.
*-----------------------------------*/

#include <I2CMultimaster.h>

#ifndef INCLUDE_THERMALSENSOR_H_
#define INCLUDE_THERMALSENSOR_H_

// Definitions
#define NUM_TEMPS 9

#define TPA81_WRITE 0xD0
#define TPA81_READ 0xD1

#define MASTER 0xC0

#define AMBIENT 0x01
#define PIXEL_1 0x02
#define PIXEL_2 0x03
#define PIXEL_3 0x04
#define PIXEL_4 0x05
#define PIXEL_5 0x06
#define PIXEL_6 0x07
#define PIXEL_7 0x08
#define PIXEL_8 0x09

// Prototypes
void initThermalSensor(void);
void ts_get_temps(uint8_t *tempArray);

#endif /* INCLUDE_THERMALSENSOR_H_ */

