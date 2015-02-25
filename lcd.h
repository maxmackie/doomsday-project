#include <stdbool.h>

void initLCD();
void TaskLCD(void *pvParameters);
void printLCD(char* message, bool interrupt);
