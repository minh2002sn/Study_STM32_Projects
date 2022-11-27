#include "button.h"

//========== btn func ==========//
__weak void btn_pressing_callback(Button_Typedef *ButtonX){
	;
}

__weak void btn_release_callback(Button_Typedef *ButtonX){
	;
}

__weak void btn_press_short_callback(Button_Typedef *ButtonX){
	;
}

__weak void btn_press_long_callback(Button_Typedef *ButtonX){
	;
}

void button_handle(Button_Typedef *ButtonX){
	uint8_t state = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
	if(state != ButtonX->btn_filter){
		ButtonX->btn_filter = state;
		ButtonX->is_debouncing = 1;
		ButtonX->time_debounce = HAL_GetTick();
	}

	if(ButtonX->is_debouncing && (HAL_GetTick() - ButtonX->time_debounce >= 15)){
		ButtonX->btn_current = ButtonX->btn_filter;
		ButtonX->is_debouncing = 0;
	}

	if(ButtonX->btn_current != ButtonX->btn_last){
		if(ButtonX->btn_current == 0){
			btn_pressing_callback(ButtonX);
			ButtonX->is_press_long = 1;
			ButtonX->time_start_press = HAL_GetTick();
		}
		else{
			if(HAL_GetTick() - ButtonX->time_start_press <= 1000){
				btn_press_short_callback(ButtonX);
			}
			btn_release_callback(ButtonX);
		}
		ButtonX->btn_last = ButtonX->btn_current;
	}

	if(ButtonX->is_press_long && (HAL_GetTick() - ButtonX->time_start_press >= 3000)){
		btn_press_long_callback(ButtonX);
		ButtonX->is_press_long = 0;
	}
}

void button_init(Button_Typedef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	ButtonX->GPIOx = GPIOx;
	ButtonX->GPIO_Pin = GPIO_Pin;
	ButtonX->btn_current = 1;
	ButtonX->btn_last = 1;
	ButtonX->btn_filter = 1;
	ButtonX->is_debouncing = 0;
	ButtonX->is_press_long = 0;

}
