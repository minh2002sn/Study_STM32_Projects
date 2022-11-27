#ifndef RESPONSE_H
#define RESPONSE_H

#include "stm32f1xx_hal.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void response_init(UART_HandleTypeDef *_huart);
void response_print(const char *str, ...);

#endif
