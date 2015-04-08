
#ifndef SERVOS_H_
#define SERVOS_H_

//Functions to control maneuvers
//all these functions call starter to set parameters for the side servos
void forward (void);
void backward (void);
void spinCC(void); //clockwise 90 degrees
void spinCW(void); //counter-clockwise 90 degrees
void stop(void);

//starter sets the parameters to start and move the side servos
//creates a task with function pointer to control() to control the speed
void starter(int id1, uint16_t pwm1, int id2, uint16_t pwm2);
//Prototype control system for the speed of the wheels
//void control ();

//Temperature servo
void scan(void); //call for continuous 180 degree right-left-right movement
void stopScan(void); //call to stop the movement; returns the sensor to center position

uint16_t const CW_PWM = 2450;
uint16_t const CC_PWM = 3000;

#endif
