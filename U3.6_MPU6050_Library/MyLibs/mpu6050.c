#include "mpu6050.h"

extern UART_HandleTypeDef huart2;

void IMU_Init(I2C_HandleTypeDef *hi2c){
	uint8_t Data = 0;

	while(Data != 0x68){
		HAL_I2C_Mem_Read(hi2c, MPU_ADDRESS, WHO_AM_I_RES, 1, &Data, 1, 1000);
	}

	Data = 0;
	HAL_I2C_Mem_Write(hi2c, MPU_ADDRESS, PWR_MGMT_1_RES, 1, &Data, 1, 1000);

	Data = 0;
	HAL_I2C_Mem_Write(hi2c, MPU_ADDRESS, GYRO_CONFIG_RES, 1, &Data, 1, 1000);

	Data = 0;
	HAL_I2C_Mem_Write(hi2c, MPU_ADDRESS, ACCEL_CONFIG_RES, 1, &Data, 1, 1000);
}

void IMU_Calibrate(I2C_HandleTypeDef *hi2c, float Acc_Error[3], float Gyro_Error[3]){
	uint8_t I2C_GYRO_Buffer[6] = {};
	uint8_t I2C_ACCEL_Buffer[6] = {};

	uint8_t c = 0;

	while(c < 200){
		HAL_I2C_Mem_Read(hi2c, MPU_ADDRESS, GYRO_XOUT_H, 1, I2C_GYRO_Buffer, 6, 1000);
		HAL_I2C_Mem_Read(hi2c, MPU_ADDRESS, ACCEL_XOUT_H, 1, I2C_ACCEL_Buffer, 6, 1000);

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

}

double IMU_Get_Data(I2C_HandleTypeDef *hi2c, IMU_AIXS_HandleTypeDef axis){
	const float Acc_Error[3] = {0.05, 0.01, 0.92};
	const float Gyro_Error[3] = {-2.71, 1.01, 1.01};
	double Acc[3] = {};
	double Gyro[3] = {};
	double IMU_Angle[3];
	uint32_t delta_time = 0;
	static uint32_t time_get_value = 0;
	static double GYRO_Angle[3] = {};
	double ACCEL_Angle[2] = {};
	uint8_t I2C_GYRO_Buffer[6] = {};
	uint8_t I2C_ACCEL_Buffer[6] = {};

	HAL_I2C_Mem_Read(hi2c, MPU_ADDRESS, GYRO_XOUT_H, 1, I2C_GYRO_Buffer, 6, 1000);
	HAL_I2C_Mem_Read(hi2c, MPU_ADDRESS, ACCEL_XOUT_H, 1, I2C_ACCEL_Buffer, 6, 1000);
	delta_time = HAL_GetTick() - time_get_value;
	time_get_value = HAL_GetTick();

	Acc[0] = (int16_t)((int16_t)I2C_ACCEL_Buffer[0] << 8 | I2C_ACCEL_Buffer[1]) / 16384.0 - Acc_Error[0];
	Acc[1] = (int16_t)((int16_t)I2C_ACCEL_Buffer[2] << 8 | I2C_ACCEL_Buffer[3]) / 16384.0 - Acc_Error[1];
	Acc[2] = (int16_t)((int16_t)I2C_ACCEL_Buffer[4] << 8 | I2C_ACCEL_Buffer[5]) / 16384.0 - Acc_Error[2];

	Gyro[0] = (int16_t)((int16_t)I2C_GYRO_Buffer[0] << 8 | I2C_GYRO_Buffer[1]) / 131.0 - Gyro_Error[0];
	Gyro[1] = (int16_t)((int16_t)I2C_GYRO_Buffer[2] << 8 | I2C_GYRO_Buffer[3]) / 131.0 - Gyro_Error[1];
	Gyro[2] = (int16_t)((int16_t)I2C_GYRO_Buffer[4] << 8 | I2C_GYRO_Buffer[5]) / 131.0 - Gyro_Error[2];

//	ACCEL_Angle[0] = atan2(Acc[1], sqrt(pow(Acc[0], 2) + pow(Acc[2], 2))) * 180 / PI;
	ACCEL_Angle[0] = atan2(Acc[1], Acc[0]) * 180 / PI;
	ACCEL_Angle[1] = -atan2(Acc[0], sqrt(pow(Acc[1], 2) + pow(Acc[2], 2))) * 180 / PI;

	GYRO_Angle[0] = GYRO_Angle[0] + delta_time / 1000.0 * Gyro[0];
	GYRO_Angle[1] = GYRO_Angle[1] + delta_time / 1000.0 * Gyro[1];
	GYRO_Angle[2] = GYRO_Angle[2] + delta_time / 1000.0 * Gyro[2];

	IMU_Angle[0] = 0.98 * GYRO_Angle[0] + 0.02 * ACCEL_Angle[0];
	IMU_Angle[1] = 0.98 * GYRO_Angle[1] + 0.02 * ACCEL_Angle[1];
	IMU_Angle[2] = 0.98 * GYRO_Angle[2] + 0.02 * ACCEL_Angle[2];

	char b[30] = {};
	sprintf(b, "%3.2f %3.2f %3.2f\n", IMU_Angle[0], IMU_Angle[1], IMU_Angle[2]);
	HAL_UART_Transmit(&huart2, b, 30, 200);


	return IMU_Angle[axis];
}
