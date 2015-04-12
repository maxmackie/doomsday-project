/*----------------------------------------------------------------------------
 * lcd.c
 *
 * @author Christopher Sluyter 6390470
 *----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "serial.h"

#include "data.h"

// Port Handles for Serial and LCD
extern xComPortHandle xSerialPort;
extern xComPortHandle xSerial1Port;

/*
 * Commands that can be transmitted to the LCD
 *
 * CLEAR: Clears the display
 * ROW_1: Sets the cursor to the beginning of the top row
 * ROW_2: Sets the cursor to the beginning of the bottom row
 * SET_ROWS: Tells the LCD to write on both rows
 */
uint8_t const CLEAR = 0x01;
uint8_t const ROW_1 = 0x80;
uint8_t const ROW_2 = 0xC0;
uint8_t const SET_ROWS = 0x38;

// Private methods
static void print(char* topMsg, char* bottomMsg);
static void transmit(uint8_t command);

/*----------------------------------------------------------------------------
 * Description:
 * 		Performs all initialization for the LCD
 *----------------------------------------------------------------------------*/
void initLCD()
{
	// Initialize the serial port handle for the LCD
	xSerial1Port = xSerialPortInitMinimal(USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);

	// Set the LCD to display 2 rows and clear it
	transmit(SET_ROWS);
	transmit(CLEAR);

	// The task is now initialized
	xSerialxPrint_P(&xSerialPort, PSTR("\nLCD Module Initialized\n"));
}

/*----------------------------------------------------------------------------
 * Description:
 * 		Displays the 8 temperature values (indices 1 through 9) of the given
 * 		tempArray on the LCD
 *
 * Parameters:
 * 		pvParameters - the array of temperatures to display
 *----------------------------------------------------------------------------*/
void taskTemperatureLCD(void *pvParameters)
{
	uint8_t *temperatureArray = (uint8_t *)pvParameters;

	// The top message is the first 4 pixel temperatures
	char* topMsg = "";
	sprintf(topMsg, "%2d %2d %2d %2d", temperatureArray[1], temperatureArray[2], temperatureArray[3], temperatureArray[4]);

	// The bottom message is the last 4 pixel temperatures
	char* bottomMsg = "";
	sprintf(bottomMsg, "%2d %2d %2d %2d", temperatureArray[5], temperatureArray[6], temperatureArray[7], temperatureArray[8]);

	// Display the temperatures on the LCD
	print(topMsg, bottomMsg);
}

/*----------------------------------------------------------------------------
 * Description:
 *		Displays the average temperature and the average speed on the LCD
 *
 * Parameters:
 * 		pvParameters - pointer to data structure containing temperature
 * 					   and movement arrays
 *----------------------------------------------------------------------------*/
void taskAveragesLCD(void *pvParameters)
{
	Data *data = (Data *)pvParameters;

	// Speed and Distance will be displayed on the top row
	char* topMsg = "";
	sprintf(topMsg, "Speed:%2f Dist:%2f", data->move.speed, data->move.distance);

	// Calculate the average of the first 4 pixel temperatures
	uint8_t averageFirst4 = 0;
	for (int i = 1; i <= 4; i++) averageFirst4 += data->temperatures[i];

	// Calculate the average of the last 4 pixel temperatures
	uint8_t averageLast4 = 0;
	for (int i = 5; i <= 8; i++) averageLast4 += data->temperatures[i];

	// Ambient and average temperatures will be displayed on the bottom row
	char* bottomMsg = "";
	sprintf(bottomMsg, "A:%2d F4:%2d L4:%2d", data->temperatures[0], averageFirst4, averageLast4);

	// Display the averages on the LCD
	print(topMsg, bottomMsg);
}

/*----------------------------------------------------------------------------
 * Description:
 *		Displays the topMsg string in the first row and the bottomMsg string
 *		in the second row of the LCD
 *
 * Parameters:
 * 		topMsg - The message to be displayed in the top row
 * 		bottomMsg - The message to be displayed in the bottom row
 *----------------------------------------------------------------------------*/
static void print(char* topMsg, char* bottomMsg)
{
	// Truncate the strings if they are too long (over 16 characters)
	if (sizeof(topMsg) > 16) topMsg[16] = '\0';
	if (sizeof(bottomMsg) > 16) bottomMsg[16] = '\0';

	// Clear the display
	transmit(CLEAR);

	// Print message for the top row
	transmit(ROW_1);
	xSerialxPrintf(&xSerial1Port, "%s", topMsg);

	// Print message for the bottom row
	transmit(ROW_2);
	xSerialxPrintf(&xSerial1Port, "%s", bottomMsg);
}

/*----------------------------------------------------------------------------
 * Description:
 *		Transmits the given command to the LCD
 *
 * Parameters:
 * 		command - The extended LCD command to be sent
 *----------------------------------------------------------------------------*/
static void transmit(uint8_t command)
{
	// Send the Command character followed by the extended LCD command
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", command);
}
