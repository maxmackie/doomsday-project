/*
 * servos.c
 *
 *  Created on: Mar 30, 2015
 *      Author: mhami077
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "servos.h"
#include "FreeRTOS.h"
#include "task.h"
#include "motion.h"

void starter(int id1, uint16_t pwm1, int id2, uint16_t pwm2)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	motion_servo_start(id1);
	motion_servo_start(id2);

	motion_servo_set_pulse_width(id1, pwm1);
	motion_servo_set_pulse_width(id2, pwm2);

	vTaskDelayUntil(&xLastWakeTime, ((pwm1 == pwm2 ? 700 : 1270) / portTICK_PERIOD_MS));
}

void forward()
{
	starter(0, CC_PWM, 1, CW_PWM);
}

void backward()
{
	starter(0, CW_PWM, 1, CC_PWM);
}

void spinCW()
{
	starter(0, CC_PWM, 1, CC_PWM);
}

void spinCC()
{
	starter(0, CW_PWM, 1, CW_PWM);
}

void stop()
{
	motion_servo_stop(0);
	motion_servo_stop(1);
}

void scan()
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int cCon = 1200;
	motion_servo_start(2);
	motion_servo_set_pulse_width(2,cCon);

	while(1){

		while(cCon < 4800){
			cCon = cCon + 10;
			if(cCon == 4810){
				break;
			}
			motion_servo_set_pulse_width(2,cCon);
			vTaskDelayUntil( &xLastWakeTime,(10/portTICK_PERIOD_MS));
		}

		while(cCon > 1100){
			cCon = cCon - 10;
			if(cCon == 1090){
				break;
			}
			motion_servo_set_pulse_width(2,cCon);
			vTaskDelayUntil( &xLastWakeTime,(10/portTICK_PERIOD_MS));
		}
	}
}

void stopScan()
{
	motion_servo_start(2500);
	motion_servo_stop(2);
}
