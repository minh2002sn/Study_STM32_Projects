#include "led_driver.h"

void set_led(uint8_t index, GPIO_PinState state){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 << index, state);
}

void toggle_led(uint8_t index){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0 << index);
}

void set_all_led(GPIO_PinState state){
	for(int i = 0; i < 8; i++){
		set_led(i, state);
	}
}
