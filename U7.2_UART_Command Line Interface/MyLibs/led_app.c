#include "led_app.h"

void LED_Init(){
	LED_Data.state = LED_IDLE_STATE;
	LED_DRIVER_Init();
}

void LED_Execute(){
	switch(LED_Data.state){
		case LED_BLINK_STATE:
			LED_Data.blink_state = !LED_Data.blink_state;
			set_all_led(LED_Data.blink_state);
			break;
		case LED_LIGHT_UP_STATE:
			break;
		default:
			break;
	}
}

void LED_Handle(){
	if(LED_Data.state != LED_IDLE_STATE){
		if(HAL_GetTick() - LED_Date.tick > LED_Date.time){
			LED_Execute();
			LED_Data.tick = HAL_GetTick();
		}
	}
}

void LED_EFFECT_Blink_All(uint32_t time){
	LED_Data.state = LED_BLINK_STATE;
	LED_Data.time = time;
}

void LED_EFFECT_Light_Up(){
	for(int i = 0; i < 8; i++){
		set_led(i, GPIO_PIN_SET);
		HAL_Delay(1000);
	}
	for(int i = 0; i < 8; i++){
		set_led(i, GPIO_PIN_RESET);
		HAL_Delay(1000);
	}
}
