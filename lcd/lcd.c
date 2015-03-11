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

	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x38);

	xSerialxPrintf(&xSerialPort, "\n%s\n", "LCD Task Initialized");
}

/*
 * Task to print the temperatures to the LCD
 */
void TaskLCD()
{
	// Display temperatures on the LCD
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x01);
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0x80);

	//for (int i = 1; i < sizeof(globalTempArray); i++)
	for (int i = 1; i <= 4; i++)
	{
		// Print the temp to the LCD
		xSerialxPrintf(&xSerial1Port, "%2d ", globalTempArray[i]);
	}

	// Display temperatures on the LCD
	xSerialxPrintf(&xSerial1Port, "%c", 0xFE);
	xSerialxPrintf(&xSerial1Port, "%c", 0xC0);

	//for (int i = 1; i < sizeof(globalTempArray); i++)
	for (int i = 5; i <= 8; i++)
	{
		// Print the temp to the LCD
		xSerialxPrintf(&xSerial1Port, "%2d ", globalTempArray[i]);
	}
}
