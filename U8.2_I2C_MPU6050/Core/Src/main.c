/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define TIME_WAIT_TO_STOP	150
#define	MPU_ADDRESS			0xD0
#define WHO_AM_I_RES		0x75
#define PWR_MGMT_1_RES		0x6B
#define SMPLRT_DIV_RES		0x19
#define GYRO_CONFIG_RES		0x1B
#define ACCEL_CONFIG_RES	0x1C
#define ACCEL_XOUT_H		0x3B
#define	GYRO_XOUT_H			0x43
#define PI					3.14159265359

typedef enum{
	MODE_NORMAL = '0',
	MODE_SPECIAL = '1',
} Move_Mode;
Move_Mode move_mode = MODE_SPECIAL;

typedef enum{
	STOP = 'S',
	MOVE_FORWARD = 'F',
	MOVE_BACKWARD = 'B',
	MOVE_RIGHT = 'R',
	MOVE_LEFT = 'L',
	MOVE_RIGHT_FORWARD = 'X',
	MOVE_RIGHT_BACKWARD = 'Y',
	MOVE_LEFT_FORWARD = 'Z',
	MOVE_LEFT_BACKWARD = 'T',
} Move_State;
Move_State move_state = STOP;
Move_State last_state = STOP;

uint32_t wait_stop_timer;

float map(int32_t n, int32_t min1, int32_t max1, int32_t min2, int32_t max2){
	return (float)((n - min1) * (max2 - min2)) / (max1 - min1) + min2;
}

void HC05_Handle(){
	uint8_t signal[2] = {(uint8_t) move_state, (uint8_t) move_mode};
	HAL_UART_Transmit(&huart1, signal, 2, 200);
}

void MPU_Init(){
	uint8_t Data = 0;

	while(Data != 0x68){
		HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, WHO_AM_I_RES, 1, &Data, 1, 1000);
	}

	Data = 0;
	HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, PWR_MGMT_1_RES, 1, &Data, 1, 1000);

//	Data = 0x07;
//	HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, SMPLRT_DIV_RES, 1, &Data, 1, 1000);

	Data = 0;
	HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, GYRO_CONFIG_RES, 1, &Data, 1, 1000);

	Data = 0;
	HAL_I2C_Mem_Write(&hi2c1, MPU_ADDRESS, ACCEL_CONFIG_RES, 1, &Data, 1, 1000);
}

void MPU_Calibration(){
	uint8_t I2C_GYRO_Buffer[6] = {};
	uint8_t I2C_ACCEL_Buffer[6] = {};
	double Acc_Error[3] = {};
	double Gyro_Error[3] = {};

	uint8_t c = 0;

	while(c < 200){
		HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, GYRO_XOUT_H, 1, I2C_GYRO_Buffer, 6, 1000);
		HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, ACCEL_XOUT_H, 1, I2C_ACCEL_Buffer, 6, 1000);

		Acc_Error[0] += (int16_t)(I2C_ACCEL_Buffer[0] << 8 | I2C_ACCEL_Buffer[1]) / 16384.0;
		Acc_Error[1] += (int16_t)(I2C_ACCEL_Buffer[2] << 8 | I2C_ACCEL_Buffer[3]) / 16384.0;
		Acc_Error[2] += (int16_t)(I2C_ACCEL_Buffer[4] << 8 | I2C_ACCEL_Buffer[5]) / 16384.0;

		Gyro_Error[0] += (int16_t)(I2C_GYRO_Buffer[0] << 8 | I2C_GYRO_Buffer[1]) / 131.0;
		Gyro_Error[1] += (int16_t)(I2C_GYRO_Buffer[2] << 8 | I2C_GYRO_Buffer[3]) / 131.0;
		Gyro_Error[2] += (int16_t)(I2C_GYRO_Buffer[4] << 8 | I2C_GYRO_Buffer[5]) / 131.0;

		c++;
	}

	Acc_Error[0] /= 200;
	Acc_Error[1] /= 200;
	Acc_Error[2] /= 200;

	Gyro_Error[0] /= 200;
	Gyro_Error[1] /= 200;
	Gyro_Error[2] /= 200;

	char b[60] = {};
	sprintf(b, "%3.2f %3.2f %3.2f %3.2f %3.2f %3.2f\n", Acc_Error[0], Acc_Error[1], Acc_Error[2], Gyro_Error[0], Gyro_Error[1], Gyro_Error[2]);
	HAL_UART_Transmit(&huart1, b, 60, 200);
}

void MPU_Handle(){
	const float Acc_Error[3] = {0.04, -0.01, 0.96};
	const float Gyro_Error[3] = {-3.44, 1.10, 0.35};
	double Acc[3];
	double Gyro[3];
	double MPU_Angle[3];
	uint32_t delta_time = 0;
	static uint32_t time_get_value = 0;
	static double GYRO_Angle[3] = {};
	double ACCEL_Angle[2] = {};
	uint8_t I2C_GYRO_Buffer[6] = {};
	uint8_t I2C_ACCEL_Buffer[6] = {};

	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, GYRO_XOUT_H, 1, I2C_GYRO_Buffer, 6, 1000);
	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDRESS, ACCEL_XOUT_H, 1, I2C_ACCEL_Buffer, 6, 1000);
	delta_time = HAL_GetTick() - time_get_value;
	time_get_value = HAL_GetTick();

	Acc[0] = (int16_t)(I2C_ACCEL_Buffer[0] << 8 | I2C_ACCEL_Buffer[1]) / 16384.0 - Acc_Error[0];
	Acc[1] = (int16_t)(I2C_ACCEL_Buffer[2] << 8 | I2C_ACCEL_Buffer[3]) / 16384.0 - Acc_Error[1];
	Acc[2] = (int16_t)(I2C_ACCEL_Buffer[4] << 8 | I2C_ACCEL_Buffer[5]) / 16384.0 - Acc_Error[2];

	Gyro[0] = (int16_t)(I2C_GYRO_Buffer[0] << 8 | I2C_GYRO_Buffer[1]) / 131.0 - Gyro_Error[0];
	Gyro[1] = (int16_t)(I2C_GYRO_Buffer[2] << 8 | I2C_GYRO_Buffer[3]) / 131.0 - Gyro_Error[1];
	Gyro[2] = (int16_t)(I2C_GYRO_Buffer[4] << 8 | I2C_GYRO_Buffer[5]) / 131.0 - Gyro_Error[2];

	ACCEL_Angle[0] = atan2(Acc[1], sqrt(pow(Acc[0], 2) + pow(Acc[2], 2))) * 180 / PI;
	ACCEL_Angle[1] = atan2(Acc[0], sqrt(pow(Acc[1], 2) + pow(Acc[2], 2))) * 180 / PI;

	GYRO_Angle[0] = GYRO_Angle[0] + delta_time / 1000.0 * Gyro[0];
	GYRO_Angle[1] = GYRO_Angle[1] + delta_time / 1000.0 * Gyro[1];
	GYRO_Angle[2] = GYRO_Angle[2] + delta_time / 1000.0 * Gyro[2];

	MPU_Angle[0] = 0.98 * GYRO_Angle[0] + 0.02 * ACCEL_Angle[0];
	MPU_Angle[1] = 0.98 * GYRO_Angle[1] + 0.02 * ACCEL_Angle[1];
	MPU_Angle[2] = 0.98 * GYRO_Angle[2] + 0.02 * ACCEL_Angle[2];

	char b[30] = {};
	sprintf(b, "%3.2f %3.2f %3.2f\n", MPU_Angle[0], MPU_Angle[1], MPU_Angle[2]);
	HAL_UART_Transmit(&huart1, b, 30, 200);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  MPU_Init();
//  MPU_Calibration();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  MPU_Handle();
	  HAL_Delay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

