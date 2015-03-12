#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "serial.h"

// Port Handles for Serial and LCD
extern xComPortHandle xSerialPort;
extern xComPortHandle xSerial1Port;

extern uint8_t globalTempArray[9];

void initLCD()
{
	// Initialize the USART Port Handles for Serial and LCD
	xSerial1Port = xSerialPortInitMinimal(USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);

	// Set the LCD to 2 rows
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x38);

	xSerialxPrintf(&xSerialPort, "\n%s\n", "LCD Task Initialized");
}

/*
 * Task to print the temperatures to the LCD
 */
void TaskLCD()
{
	// Clear LCD and set cursor to first position
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x01);
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x80);

	// Prints all the temperatures
	for (int i = 1; i < sizeof(globalTempArray); i++)
	{
		if (i == 5)
		{
			// Move cursor to second row
			xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
			xSerialxPrintf(&xSerial1Port, "%c", 0xC0);
		}

		// Print the temp to the LCD
		xSerialxPrintf(&xSerial1Port, "%2d ", globalTempArray[i]);
	}
}
