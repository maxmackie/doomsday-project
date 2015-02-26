#ifndef LCD_H
#define LCD_H_

#include <stdbool.h>

void initLCD();
void TaskLCD(void *pvParameters);
void printLCD(char* message, bool interrupt);

#endif
