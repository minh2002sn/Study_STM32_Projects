#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"
#include "math.h"

#define	MPU_ADDRESS			(0x68 << 1)
#define WHO_AM_I_RES		0x75
#define PWR_MGMT_1_RES		0x6B
#define SMPLRT_DIV_RES		0x19
#define GYRO_CONFIG_RES		0x1B
#define ACCEL_CONFIG_RES	0x1C
#define ACCEL_XOUT_H		0x3B
#define	GYRO_XOUT_H			0x43
#define PI					3.14159265359

typedef enum{
	ROLL,		// Roll around Ox axis
	PITCH,		// Roll around Oy axis
	YAW,		// Roll around Oz axis
}IMU_AIXS_HandleTypeDef;

void IMU_Init(I2C_HandleTypeDef *hi2c);
void IMU_Calibrate(I2C_HandleTypeDef *hi2c, float Acc_Error[3], float Gyro_Error[3]);
double IMU_Get_Data(I2C_HandleTypeDef *hi2c, IMU_AIXS_HandleTypeDef axis);

#endif
