/*---------------------------------------------*
 * File name: wifi.h                           *
 * Author: Maximillien Courchesne-Mackie       *
 * Description: Interact with the wifi shield  *
 *---------------------------------------------*/

#ifndef INCLUDE_WIFI_H_
#define INCLUDE_WIFI_H_

// Prototypes
void initLEDsOutput(void);
void toggleRed(int state);
void toggleBlue(int state);
void toggleGreen(int state);
void taskTemperatureLED(void);

#endif /* INCLUDE_WIFI_H_ */
