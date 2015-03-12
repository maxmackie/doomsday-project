
#include <stdlib.h>
#include <stdbool.h>
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


extern xComPortHandle xSerialPort;

uint8_t globalTempArray[9];

static void Runner (void *pvParameters);
typedef void (*TASK)(void); // TASK is now the type  �pointer to function�

uint8_t MINOR_CYCLE_TIME = 2;

#define TABLE_SIZE 5
TASK table[] =
{
	    ts_get_temps,
		TaskLCD,
		taskTemperatureLED
};

//#define MINOR_CYCLE_TIME 50 // millisecs

int main(void) __attribute__((OS_main));

int main(void)
{
	xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	initThermalSensor();
	initLCD();
	initLEDsOutput();

	xTaskCreate(Runner, (const portCHAR *)"TeamMax", 256, NULL, 3, NULL);

	vTaskStartScheduler();
}

static void Runner (void *pvParameters) // LOOP
{
	(void) pvParameters;;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	int i = 0 ;

	while (1){

		table[i]();

		vTaskDelayUntil( &xLastWakeTime,(MINOR_CYCLE_TIME/portTICK_PERIOD_MS)); // Sets minor cycle time

		if (i < 2){
			i ++;
		}
		else {
			i = 0;
		}
	}
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
		portCHAR *pcTaskName )
{
	//let all the LEDs be on if overflow occurs
	/* DDRE  |= _BV(DDE3);
	PORTE |= _BV(PORTE3);
	DDRE  |= _BV(DDE5);
	PORTE |= _BV(PORTE5);
	DDRH  |= _BV(DDH3);
	PORTH |= _BV(PORTH3); */
	while(1);
}
