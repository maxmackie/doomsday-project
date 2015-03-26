/*---------------------------------------------*
 * File name: wifi.h                           *
 * Author: Maximillien Courchesne-Mackie       *
 * Description: Interact with the wifi shield  *
 *---------------------------------------------*/

#ifndef INCLUDE_WIFI_H_
#define INCLUDE_WIFI_H_

/*---------------------------------------------*
 * Update LED DDR to set them as outputs       *
 *---------------------------------------------*/
void initLED(void);

/*---------------------------------------------*
 * Toggle the associated colour LED to either  *
 * on (state=1) or off (state=0)               *
 *---------------------------------------------*/
void toggleRed(int state);
void toggleBlue(int state);
void toggleGreen(int state);

/*---------------------------------------------*
 * Task for changing based on wheel speed      *
 *---------------------------------------------*/
void taskMovementLED(uint8_t movementArray[9]);

/*---------------------------------------------*
 * Task for changing based on temperature      *
 *---------------------------------------------*/
void taskTemperatureLED(uint8_t temperatureArray[9]);

#endif /* INCLUDE_WIFI_H_ */
