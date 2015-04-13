/*-----------------------------------
* File name: sensor.c
* Author: Sameid Usmani
* Description: A short description of the module.
*-----------------------------------*/

#include <I2CMultimaster.h>
#include "sensor.h"
#include "servos.h"
#include "data.h"


int scanning = 0;
TaskHandle_t scanHandler;

uint8_t getDataFromRegister (uint8_t);
void performScan(void);


/*-----------------------
* Function: initThermalSensor
* Return value: none
* Description: Will initialize the MASTER device on the I2C Bus, also enables interrupts to allows reads to occur
*----------------------*/
void initThermalSensor(void){
	I2C_Master_Initialise(MASTER); //Initialize the MASTER device which is the controller, used to also enable interrupts on READs
}

/*-----------------------
* Function: ts_get_temps
* Return value: none, but the globalTempArray is allocated with temperatures, array is of size 9
* Description: Will get the temperature readings from the TPA81 and assign each element in the tempArray with the correct values
*----------------------*/
void taskReadTemperatures(void *pvParameters){
	struct Data *data = (struct Data *)pvParameters;
	for (int i = 0 ; i < NUM_TEMPS; i++){
		data->temperatures[i] = getDataFromRegister(AMBIENT + i);
	};
}

/*---------------------
 * Function: taskPerformThermalScan
 * Return value: none
 * Description: Will perform continous thermal scanning by rotating the TPA81 Servo by turning left to right and back. Uses our servos.h module, runs as a forked process
 * 				will only actually perform forked process if it is not already running.
 *--------------------*/

void taskPerformThermalScan(void *pvParameters){
	if (scanning == 0){
		xTaskCreate(performScan, "Fork of Thermal Scan", 256, NULL, 3, &scanHandler);
		scanning = 1;
	}
}

/*---------------------
 * Function: performScan
 * Return value: none
 * Description: Will call actual scan function from servos.h, used as a callback for creating the thermal scan task
 *--------------------*/
void performScan (void){
	scan();
}

/*---------------------
 * Function: taskStopThermalScan
 * Return value: none
 * Description: Will stop scan task and set the pulse width to the motor to stop rotating, using the stopScan function from servos.h
 *--------------------*/
void taskStopThermalScan(void *pvParameters){
	if (scanning == 1){
		stopScan();
		vTaskDelete(scanHandler);
		scanning = 0;
	}
}


/*-----------------------
* Function: getDataFromRegister
* Return value: Will return uint8_t
* Description: The function will retrieve data fromt eh
*
* 1. Send a start sequence.
* 2. Send 0xD0 (I2C address of thermal sensor with R/W bit low)
* 3. Send 0x01 (Internal address of the Ambient Temperature Register)
* 4. Send a start sequence again. (Quirk in the I2C Protocol in terms of reading)
* 5. Send 0xD1 (I2C address of thermal sensor with R/W bit high)
* 6. Read data byte from the thermal sensor.
* 7. Send a stop sequence.
*
*----------------------*/
uint8_t getDataFromRegister(uint8_t bearingRegister){

	uint8_t commandBuffer [2]; //Create a command buffer that will send a WRITE command addressing the appropriate REGISTER
	commandBuffer[0] = TPA81_WRITE; // Set the first byte in the message to WRITE to the TPA81 on the I2C Bus
	commandBuffer[1] = bearingRegister; // Set the second byte in the message to the Bearing Register on the device that will be addressed

	uint8_t readBuffer [2]; //Create a second buffer used to read data from the registers on the device
	readBuffer[0] = TPA81_READ; //Set the first byte in the message to signal a READ from the TPA81 on the i2c Bus

	I2C_Master_Start_Transceiver_With_Data( (uint8_t *)&commandBuffer, 2 );//Send the commandBuffer on the bus

	if (I2C_Check_Free_After_Stop() == pdTRUE )// i think we dont need this, as it's for a multi-master deally
		I2C_Master_Start_Transceiver_With_Data( (uint8_t *)&readBuffer, 2 ); //Send the readBuffer on the bus

	if( I2C_Master_Get_Data_From_Transceiver( (uint8_t *)&readBuffer, 2) )//Actually read in the value of the bearingRegister into the readBuffer
		return readBuffer[1];
	else
		return -1;//if in the case that the read fails, simply return -1

}
