/*-----------------------------------
* File name: sensor.c
* Author: Sameid Usmani
* Description: A short description of the module.
*-----------------------------------*/

#include <I2CMultimaster.h>
#include "sensor.h"

/*-----------------------
* Function: getAmbientTemperature
* Return value: Will return a uint8_t
* Description: Is simply a wrapper function for the "getDataFromRegister", it will simply give you the Ambient Temperature that the Thermal Sensor is picking up.
*----------------------*/
uint8_t getAmbientTemperature(void){
	return getDataFromRegister(AMBIENT);
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
uint8_t getDataFromRegister (uint8_t bearingRegister){

	uint8_t commandBuffer [2]; //Create a command buffer that will send a WRITE command addressing the appropriate REGISTER
	commandBuffer[0] = TPA81_WRITE; // Set the first byte in the message to WRITE to the TPA81 on the I2C Bus
	commandBuffer[1] = bearingRegister; // Set the second byte in the message to the Bearing Register on the device that will be addressed

	uint8_t readBuffer [2]; //Create a second buffer used to read data from the registers on the device
	readBuffer[0] = TPA81_READ; //Set the first byte in the message to signal a READ from the TPA81 on the i2c Bus

	I2C_Master_Initialise(MASTER); //Initialize the MASTER device which is the controller, used to also enable interrupts on READs
	I2C_Master_Start_Transceiver_With_Data( (uint8_t *)&commandBuffer, 2 );//Send the commandBuffer on the bus

	if (I2C_Check_Free_After_Stop() == pdTRUE )// i think we dont need this, as it's for a multi-master deally
		I2C_Master_Start_Transceiver_With_Data( (uint8_t *)&readBuffer, 2 ); //Send the readBuffer on the bus

	if( I2C_Master_Get_Data_From_Transceiver( (uint8_t *)&readBuffer, 2) )//Actually read in the value of the bearingRegister into the readBuffer
		return readBuffer[1];//return command buffer
	else
		return -1;//if in the case that the read fails, simply return -1

}
