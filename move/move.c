/*
 * move.c
 *
 *  Created on: Apr 1, 2015
 *      Author: mhami077
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "servos.h"
#include "motion.h"
#include "data.h"

int32_t errorLeft = 0;
int32_t errorRight = 0;

uint32_t const REF_LEFT = 45000;
uint32_t const REF_RIGHT = 47000;

uint32_t pwmDefaultLeft;
uint32_t pwmDefaultRight;

uint32_t pwmLeft = 0;
uint32_t pwmRight = 0;

TaskHandle_t controllerHandle;
TaskHandle_t measureHandle;

float const GAIN = 0.002;

Type pattern[] =
{
		FORWARD,
		STOP,
		BACKWARD,
		STOP,
		SPIN_CW,
		STOP,
		SPIN_CCW,
		STOP
};

static void controller();
static void measure(void *pvParameters);

void maneuvers(void *pvParameters)
{
	Move *move = (Move *)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	patternLoop: for (int i = 0;; i = (i + 1) % sizeof(pattern))
	{
		move->type = pattern[i];

		switch (move->type)
		{
		case FORWARD:
			pwmDefaultLeft = CC_PWM;
			pwmDefaultRight = CW_PWM;
			forward();
			break;
		case BACKWARD:
			pwmDefaultLeft = CW_PWM;
			pwmDefaultRight = CC_PWM;
			backward();
			break;
		case SPIN_CW:
			pwmDefaultLeft = CC_PWM;
			pwmDefaultRight = CC_PWM;
			spinCC();
			break;
		case SPIN_CCW:
			pwmDefaultLeft = CW_PWM;
			pwmDefaultRight = CW_PWM;
			spinCW();
			break;
		case STOP:
			vTaskDelayUntil(&xLastWakeTime, (10 / portTICK_PERIOD_MS));
			goto patternLoop;
		}

		xTaskCreate(measure, (const portCHAR *)"Measure", 256, (void *)&move, 1, &measureHandle);
		xTaskCreate(controller, (const portCHAR *)"Controller", 256, NULL, 2, &controllerHandle);

		// TODO: Change the delay according to the time needed for the robot to perform a movement
		vTaskDelayUntil(&xLastWakeTime, (10 / portTICK_PERIOD_MS));

		vTaskDelete(&controllerHandle);
		vTaskDelete(&measureHandle);
	}
}

static void controller()
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	motion_servo_set_pulse_width(0, pwmLeft);
	motion_servo_set_pulse_width(1, pwmRight);

	vTaskDelayUntil(&xLastWakeTime, (10 / portTICK_PERIOD_MS));

	pwmLeft = pwmDefaultLeft - (GAIN * errorLeft);
	pwmRight = pwmDefaultRight + (GAIN * errorRight);
}

static void measure(void *pvParameters)
{
	Move *move = (Move *)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	uint32_t encodedLeft = 0;
	uint32_t encodedRight = 0;

	uint32_t fetch = 0;
	for (int i = 0; i <= 32; i++)
	{
		motion_enc_read(0, &fetch);
		encodedLeft += fetch;

		motion_enc_read(1, &fetch);
		encodedRight += fetch;
	}

	encodedLeft /= 32;
	encodedRight /= 32;

	errorLeft = REF_LEFT - encodedLeft;
	errorRight = REF_RIGHT - encodedRight;

	double speedLeft = 17.28 / (encodedLeft * 0.00000005);
	double speedRight = 17.28 / (encodedRight * 0.00000005);

	move->speed = (speedLeft + speedRight) / 2;
	move->distance += 17.28;

	vTaskDelayUntil(&xLastWakeTime, (10 / portTICK_PERIOD_MS));
}
