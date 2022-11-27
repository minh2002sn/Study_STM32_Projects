#include "response.h"

static UART_HandleTypeDef *huart;

void response_init(UART_HandleTypeDef *_huart){
	huart = _huart;
}

void response_print(const char *str, ...){
	char stringArray[50];

	va_list args;
	va_start(args, str);
	vsprintf(stringArray, str, args);
	va_end(args);

	HAL_UART_Transmit(huart, (uint8_t *)stringArray, strlen(stringArray), 100);
}
