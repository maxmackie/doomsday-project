/*---------------------------------------------*
 * File name: wifi.c                           *
 * Author: Maximillien Courchesne-Mackie       *
 * Description: Interact with the wifi shield  *
 *---------------------------------------------*/

#include "wifi.h";
#include "data.h";

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
 *         If any other state variable is given, simply toggle     *
 *         the state of the LED.
 *-----------------------------------------------------------------*/
void toggleRed(int state) {
  if (state == 0) {
    PORTH |= _BV(PORTH3);
  } else if (state == 1) {
    PORTH &= ~_BV(PORTH3);
  } else {
		if (_BV(PORTH3) == 1) {
			PORTH |= _BV(PORTH3); // turn off
		} else {
			PORTH &= ~_BV(PORTH3); // turn on
		}
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
 * Function: taskMovementLED                                       *
 * Returns:  none                                                  *
 * Desc: Updates the LED based on how the robot is moving          *
 *-----------------------------------------------------------------*/
void taskMovementLED(void *pvParameters) {

	Move *data = (Move*) pvParameters;
	Type type = data->type;

	switch (type) {
		case FORWARD:
			toggleRed(0);
			toggleBlue(0);
			toggleGreen(1);
			break;
		case BACKWARD:
			toggleRed(2); // toggle the state
			toggleBlue(0);
			toggleGreen(0);
			break;
		case SPIN_CW:
		case SPIN_CCW:
			toggleRed(0);
			toggleBlue(1);
			toggleGreen(0);
			break;
		case STOP:
			toggleRed(0);
			toggleBlue(0);
			toggleGreen(0);
			break;
	}

	return;
}
