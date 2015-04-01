#ifndef LCD_H_
#define LCD_H_

/*----------------------------------------------------------------------------
 * Description:
 * 		Performs all initialization for the LCD
 *----------------------------------------------------------------------------*/
void initLCD(void);

/*----------------------------------------------------------------------------
 * Description:
 * 		Displays the 8 temperature values (indices 1 through 9) of the given
 * 		tempArray on the LCD
 *
 * Parameters:
 * 		pvParameters - the array of temperatures to display
 *----------------------------------------------------------------------------*/
void taskTemperatureLCD(void *pvParameters);

/*----------------------------------------------------------------------------
 * Description:
 *		Displays the average temperature and the average speed on the LCD
 *
 * Parameters:
 * 		pvParameters - pointer to data structure containing temperature
 * 					   and movement arrays
 *----------------------------------------------------------------------------*/
void taskAveragesLCD(void *pvParameters);

#endif
