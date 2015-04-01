/*----------------------------------------------------------------------------
 * main.c
 *----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"
#include "wifi.h"
#include "therm.h"
#include "serial.h"
#include "data.h"

// USART Serial Port Handle
extern xComPortHandle xSerialPort;

// Data object with temperature and speed arrays
Data data;

/*----------------------------------------------------------------------------
 * Description:
 * 		Main function that initializes the modules and creates/runs the tasks
 *----------------------------------------------------------------------------*/
int main(void) __attribute__((OS_main));
int main()
{
	// Initialize serial port handle
	xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);

	// Initialize all the modules
	initThermalSensor();
	initLED();
	initLCD();

	// Create the tasks
	xTaskCreate(ts_get_temps, (const portCHAR *)"Thermal", 256, (void *)data.temperatures, 3, NULL);
	xTaskCreate(taskMovementLED, (const portCHAR *)"LED", 256, (void *)&data.move, 3, NULL);
	xTaskCreate(taskAveragesLCD, (const portCHAR *)"LCD", 256, (void *)&data, 3, NULL);

	// Start the scheduler
	vTaskStartScheduler();
}

/*----------------------------------------------------------------------------
 * Description:
 * 		...
 *----------------------------------------------------------------------------*/
void vApplicationStackOverflowHook(TaskHandle_t xTask, portCHAR *pcTaskName) { while(1); }
