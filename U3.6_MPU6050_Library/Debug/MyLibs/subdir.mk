################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyLibs/mpu6050.c 

OBJS += \
./MyLibs/mpu6050.o 

C_DEPS += \
./MyLibs/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
MyLibs/%.o: ../MyLibs/%.c MyLibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/A_Minh/B_STM32_Projects/StudySTM32/U3.6_MPU6050_Library/MyLibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyLibs

clean-MyLibs:
	-$(RM) ./MyLibs/mpu6050.d ./MyLibs/mpu6050.o

.PHONY: clean-MyLibs

