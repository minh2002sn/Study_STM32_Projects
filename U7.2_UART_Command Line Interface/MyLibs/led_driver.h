#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "stm32f1xx_hal.h"
#include "stdint.h"

void LED_DRIVER_Init();

void set_led(uint8_t index, GPIO_PinState state);
void toggle_led(uint8_t index);
void set_all_led(GPIO_PinState state);

#endif
