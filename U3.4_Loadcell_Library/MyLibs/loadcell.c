#include "loadcell.h"

extern UART_HandleTypeDef huart2;

void LC_Init(LC_HandleTypeDef *hlc, GPIO_TypeDef *DT_GPIOx, uint16_t DT_GPIO_Pin, GPIO_TypeDef *CLK_GPIOx, uint16_t CLK_GPIO_Pin){
	hlc->DT_GPIOx = DT_GPIOx;
	hlc->DT_GPIO_Pin = DT_GPIO_Pin;
	hlc->CLK_GPIOx = CLK_GPIOx;
	hlc->CLK_GPIO_Pin = CLK_GPIO_Pin;
	hlc->calib_state = NO_CALIBRATE;
//	uint32_t data = 0b00111001111001000010000000111001;
	hlc->a = 0.000437;
	hlc->b = -3916.127441;
	hlc->b += 3;
}

long LC_Read(LC_HandleTypeDef *hlc){
	long res = 0;
	uint8_t data[3] = {};
	HAL_GPIO_WritePin(hlc->CLK_GPIOx, hlc->CLK_GPIO_Pin, GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(hlc->DT_GPIOx, hlc->DT_GPIO_Pin) == GPIO_PIN_SET);
	for(int i = 0; i < 3; i++){
	  for(int j = 0; j < 8; j++){
		HAL_GPIO_WritePin(hlc->CLK_GPIOx, hlc->CLK_GPIO_Pin, GPIO_PIN_SET);
		data[2-i] |= HAL_GPIO_ReadPin(hlc->DT_GPIOx, hlc->DT_GPIO_Pin) << (7-j);
		HAL_GPIO_WritePin(hlc->CLK_GPIOx, hlc->CLK_GPIO_Pin, GPIO_PIN_RESET);
	  }
	}
	HAL_GPIO_WritePin(hlc->CLK_GPIOx, hlc->CLK_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hlc->CLK_GPIOx, hlc->CLK_GPIO_Pin, GPIO_PIN_RESET);

	data[2] ^= 0x80; // if (MSB == 1) -> MSB = 0;
	res = ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
	return res;
}

void LC_Calibration(LC_HandleTypeDef *hlc){
	float y1 = 100.0;
	float x1 = 0;
	float x0 = 0;

	HAL_UART_Transmit(&huart2, "Getting x0\n", 11, 100);

	hlc->calib_state = GETTING_X0;
	for(int i = 0; i < NUMBER_OF_SAMPLE; i++){
		x0 += LC_Read(hlc);
		HAL_Delay(10);
	}
	x0 /= (float)NUMBER_OF_SAMPLE;

	hlc->calib_state = WAITING_SAMPLE_MASS;
	HAL_UART_Transmit(&huart2, "Waiting sample mass\n", 20, 100);
	while(1){
		if(LC_Read(hlc) > x0 + 10000){
			HAL_UART_Transmit(&huart2, "Getting x1\n", 11, 100);
			hlc->calib_state = GETTING_X1;
			HAL_Delay(2000);
			for(int i = 0; i < NUMBER_OF_SAMPLE; i++){
				x1 += LC_Read(hlc);
				HAL_Delay(10);
			}
			x1 /= (float)NUMBER_OF_SAMPLE;
			break;
		}
	}
	hlc->a = y1 / (x1 - x0);
	hlc->b = -y1 * x0 / (x1 - x0);
	uint8_t *data;
	data = &(hlc->a);
	uint8_t Tx_Buff[20] = {};
//	sprintf(Tx_Buff, "%x %x %x %x\n", data[3], data[2], data[1], data[0]);
//	HAL_UART_Transmit(&huart2, Tx_Buff, 20, 500);
//	data = &(hlc->b);
//	sprintf(Tx_Buff, "%x %x %x %x\n", data[3], data[2], data[1], data[0]);
//	HAL_UART_Transmit(&huart2, Tx_Buff, 20, 500);
	sprintf(Tx_Buff, "%lf ", 20, hlc->a);
	HAL_UART_Transmit(&huart2, Tx_Buff, 20, 500);
	sprintf(Tx_Buff, "%lf\n", 20, hlc->b);
	HAL_UART_Transmit(&huart2, Tx_Buff, 20, 500);
	HAL_UART_Transmit(&huart2, "Calibrate done\n", 11, 100);

	hlc->calib_state = CALIBRATED;
}

float LC_Get_Mass(LC_HandleTypeDef *hlc){
	return hlc->a * LC_Read(hlc) + hlc->b;
}
