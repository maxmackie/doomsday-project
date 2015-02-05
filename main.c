
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "serial.h"

extern xComPortHandle xSerialPort;

static void taskLEDControl (void *pvParameters);

static QueueHandle_t ipc;

struct structMsg
{
   unsigned long ambiant;
   unsigned long pixels[];
} msg;

int main(void) __attribute__((OS_main));

int main(void)
{
	// serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);

	// Create the queue from the sensor to the LED and the sensor to the LCD
	struct structMsg tmp;
	ipc = xQueueCreate(1, sizeof(tmp));
	if (ipc == 0) {
		// The queue was not created and there was an issue
		avrSerialPrint_P(PSTR("\r\n\n\n ERROR: Could not create the IPC queue \r\n"));
	}

    // Create the LED task
    xTaskCreate(taskLEDControl, "LED Task", 256, NULL, 3, NULL);

	vTaskStartScheduler();
}

static void taskLEDControl(void *pvParameters)
{
	// The usual stuff
    (void) pvParameters;
    TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	// Set port values
	PORTE |= _BV(PORTE3); //red
	PORTE |= _BV(PORTE5);
	PORTH |= _BV(PORTH3);

	// Task loop
	while (1) {
		struct structMsg *val;
		if (ipc != 0) {
			if (xQueuePeek(ipc, &(val), (TickType_t) 10)) {
				// Received a value from the front of the queue

			}
		}
	}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, portCHAR *pcTaskName)
{
	// Print console debugging info if we have overflow
	avrSerialPrint_P(PSTR("\r\n\n\n ERROR: Overflow \r\n"));
	while(1);
}

/*
static void TaskColors (void *pvParameters) // LOOP
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();

	DDRE |= _BV(DDE3); // green
	DDRE |= _BV(DDE5); // blue
	DDRH |= _BV(DDH3); // red

	PORTE |= _BV(PORTE3);
	PORTE |= _BV(PORTE5);
	PORTH |= _BV(PORTH3);

    while(1)
    {


		PORTE &= ~_BV(PORTE3);       //GRREN ON
				vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) ); ////delay red

	PORTH &= ~_BV(PORTH3);       // RED ON
				vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) );///delay blue

	PORTE &= ~_BV(PORTE5);       // BLUE ON
			vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) ); ///delay green

	PORTE |=  _BV(PORTE3);       // GREEN OFF
			vTaskDelayUntil( &xLastWakeTime, ( 200  / portTICK_PERIOD_MS ) );

	PORTH |=  _BV(PORTH3);       // RED OFF
			vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) );

	PORTE |=  _BV(PORTE5);       // BLUE OFF
			vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) );

    }
}
*/


