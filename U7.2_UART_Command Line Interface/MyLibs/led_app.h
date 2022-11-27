#ifndef LAD_APP_H
#define LED_APP_H

#include "led_driver.h"

typedef enum{
	LED_IDLE_STATE,
	LED_BLINK_STATE,
	LED_LIGHT_UP_STATE,
} LED_STATE_HandleTypeDef;

typedef struct{
	LED_STATE_HandleTypeDef state;
	uint32_t time;
	uint32_t tick;

	uint8_t blink_state = 0;

}LED_DATA_HandleTypdeDef;

LED_DATA_HandleTypdeDef LED_Data;

void LED_Init();

void LED_Execute();
void LED_Handle();
void LED_EFFECT_Blink_All(uint32_t time);
void LED_EFFECT_Light_Up();

#endif
