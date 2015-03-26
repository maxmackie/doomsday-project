/*---------------------------------------------*
 * File name: wifi.c                           *
 * Author: Maximillien Courchesne-Mackie       *
 * Description: Interact with the wifi shield  *
 *---------------------------------------------*/

#include "wifi.h";

extern uint8_t globalTempArray[9];

/*-----------------------------------------------------------------*
 * Function: initLEDs                                              *
 * Returns:  none                                                  *
 * Desc:   Sets the DDR for the LEDs to output                     *
 *-----------------------------------------------------------------*/
void initLED() {
  DDRH |= _BV(DDH3); // red
  DDRE |= _BV(DDE5); // blue
  DDRE |= _BV(DDE3); // green
}

/*-----------------------------------------------------------------*
 * Function: toggleRed                                             *
 * Returns:  none                                                  *
 * Desc:   Sets the red LED to ON (1) or OFF (0)                   *
 *-----------------------------------------------------------------*/
void toggleRed(int state) {
  if (state == 0) {
    PORTH |= _BV(PORTH3);
  } else {
    PORTH &= ~_BV(PORTH3);
  }
}

/*-----------------------------------------------------------------*
 * Function: toggleBlue                                            *
 * Returns:  none                                                  *
 * Desc:   Sets the blue LED to ON (1) or OFF (0)                  *
 *-----------------------------------------------------------------*/
void toggleBlue(int state) {
  if (state == 0) {
    PORTE |= _BV(PORTE5);
  } else {
    PORTE &= ~_BV(PORTE5);
  }
}

/*-----------------------------------------------------------------*
 * Function: toggleGreen                                           *
 * Returns:  none                                                  *
 * Desc:   Sets the green LED to ON (1) or OFF (0)                 *
 *-----------------------------------------------------------------*/
void toggleGreen(int state) {
  if (state == 0) {
    PORTE |= _BV(PORTE3);
  } else {
    PORTE &= ~_BV(PORTE3);
  }
}

/*-----------------------------------------------------------------*
 * Function: taskTemperatureLED                                    *
 * Returns:  none                                                  *
 * Desc: Takes a pointer to an array of 8 uint8_t values           *
 *       representing the 8 pixel temperature values. These        *
 *       values are averaged and the LED colour is changed.        *
 *-----------------------------------------------------------------*/
void taskTemperatureLED(uint8_t temperatureArray[9]) {
  while (1) {
    // calculate the average of the pixel temperatures
    uint8_t total = 0;
    for (int i = 0; i < 8; i++) { total += temperatureArray[i]; }
    float avg = total / 8;

    if (avg < 20 && avg > 10) {
      // turn on blue and turn off others
      toggleRed(0);
      toggleBlue(1);
      toggleGreen(0);
    } else if (avg > 30 || avg < 10) {
      // turn on green and turn off others
      toggleRed(1);
      toggleBlue(0);
      toggleGreen(0);
    } else {
      // turn on red and turn off others
      toggleRed(0);
      toggleBlue(0);
      toggleGreen(1);
    }
  }
}

/*-----------------------------------------------------------------*
 * Function: taskMovementLED                                       *
 * Returns:  none                                                  *
 * Desc: Updates the LED based on how the robot is moving          *
 *-----------------------------------------------------------------*/
void taskMovementLED(uint8_t movementArray[9]) {
	return;
}
