#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "serial.h"
#include "global.h"

// Port Handles for Serial and LCD
extern xComPortHandle xSerialPort;
extern xComPortHandle xSerial1Port;

extern uint8_t* tempArray;

void printLCD(char* message, bool interrupt);

void initLCD()
{
	// Initialize the USART Port Handles for Serial and LCD
	xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	xSerial1Port = xSerialPortInitMinimal(USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
}

/*
 * Task to print the temperatures to the LCD
 *
 * @param array - Pointer to temperature array
 */
void TaskLCD(uint8_t* array)
{
	// Task Loop
	while (1)
	{
		// Message to be displayed
		char* message = "";

		for (int i = 0; i < sizeof(array); i++)
		{
			// Get the temperature as a string
			char* num = "";
			itoa(array[i], num, 10);

			// Append to the message
			strcat(message, num);
			strcat(message, " ");
		}

		// Display temperatures on the LCD
		printLCD(message, true);
	}
}

/*
 * Prints the message to the LCD
 *
 * @param message - The message to print
 * @param interrupt - Indicates whether or not the function will use an interrupt
 */
void printLCD(char* message, bool interrupt)
{
	// We can only display 32 characters
	if (strlen(message) >  32)
	{
		// If there are more than 32 characters, only display the first 32
		strncpy(message, message, 32);
		message[32] = '\0';
	}

	// Calculate the remaining space to fill on the LCD
	int whiteSpace = 32 - strlen(message);

	// Create pointer to the Serial Print function with or without interrupt
	void (*serialPrint)(xComPortHandlePtr, const char*, ...);
	serialPrint = interrupt ? &xSerialxPrintf : &avrSerialxPrintf;

	// Print the message to the LCD
	(*serialPrint)(&xSerial1Port, "%s", message);

	// Print white space to set the cursor back to the beginning
	for (int i = 0; i < whiteSpace; i++)
	{
		(*serialPrint)(&xSerial1Port, " ");
	}
}
