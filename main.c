
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "serial.h"
#include "sensor.h"

extern xComPortHandle xSerialPort;

// Static declarations of the task functions
static void taskLEDControl (void *pvParameters);
static void taskThermalSensor (void *pvParameters);

// The queue used for IPC between the sensor, LCD and LED tasks
static QueueHandle_t ipc;

// Define the structure that represents a single reading from
// the temperature sensor. The struct has a value for the ambient
// temperature aswell as an array of values that represent the
// pixel temperatures
struct structMsg
{
	unsigned long ambient;
	unsigned long pixels[];
} msg;

int main(void) __attribute__((OS_main));

int main(void)
{

	// Set up the serial port communications channel
    xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
    
    // Display a polite welcome message - we are Canadians after all
    avrSerialPrint_P(PSTR("\r\n Successfully running Doomsday Project \r\n"));

	// Create the queue from the sensor to the LED and the sensor to the LCD
	struct structMsg tmp;
	ipc = xQueueCreate(1, sizeof(tmp));
	if (ipc == 0) {
		// The queue was not created and there was an issue
		avrSerialPrint_P(PSTR("\r\n ERROR: Could not create the IPC queue \r\n"));
	}

	// Create the LED task
    xTaskCreate(taskLEDControl, "LED Task", 256, NULL, 3, NULL);
    // Create the ThermalSensor task
    xTaskCreate(taskThermalSensor, "Thermal Sensor Task", 256, NULL, 3, NULL);

	// TODO: Modify the default scheduler with our own one
	vTaskStartScheduler();
}

// The LED task will peek at the first value in the global queue and update
// the projected colour based on what it reads
static void taskLEDControl(void *pvParameters)
{
	// The usual stuff
	(void) pvParameters;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	// Turn off the LED colours
	PORTE |= _BV(PORTE3); // green
	PORTE |= _BV(PORTE5); // blue
	PORTH |= _BV(PORTH3); // red

	// Task loop
	while (1) {
		struct structMsg *val;
		if (ipc != 0) {
			// Attempt to peek at the first value in the queue
			if (xQueuePeek(ipc, &(val), (TickType_t) 10)) {
				// calculate the average value
				// ...
				unsigned long avg = 0;
				if (avg < 20) {
					// turn on blue and turn off others
					PORTE |= _BV(PORTE3);
					PORTE &= ~_BV(PORTE5);
					PORTH |= _BV(PORTH3);
				} else if (avg < 30) {
					// turn on green and turn off others
					PORTE &= ~_BV(PORTE3);
					PORTE |= _BV(PORTE5);
					PORTH |= _BV(PORTH3);
				} else {
					// turn on red and turn off others
					PORTE |= _BV(PORTE3);
					PORTE |= _BV(PORTE5);
					PORTH &= ~_BV(PORTH3);
				}
			}
		}
	}
}

static void taskThermalSensor(void *pvParameters)
{
    (void)pvParameters;
    
    uint8_t test = getAmbientTemperature();
    avrSerialPrintf_P(PSTR("\r\nAmbient Temperature: %u \r\n "), test);

}

// Handles stack overflows by turning off the LEDs and logging an error
void vApplicationStackOverflowHook(TaskHandle_t xTask, portCHAR *pcTaskName)
{
	// Turn off the LEDs
	PORTE |= _BV(PORTE3); // green
	PORTE |= _BV(PORTE5); // blue
	PORTH |= _BV(PORTH3); // red

	// Print console debugging info if we have overflow
	avrSerialPrint_P(PSTR("\r\n ERROR: Overflow \r\n"));
	while(1);
}


