################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyLibs/LCD_I2C.c 

OBJS += \
./MyLibs/LCD_I2C.o 

C_DEPS += \
./MyLibs/LCD_I2C.d 


# Each subdirectory must supply rules for building sources it contributes
MyLibs/%.o: ../MyLibs/%.c MyLibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -c -I"E:/A_Minh/B_STM32_Projects/StudySTM32/U3.2_LCD Library/MyLibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyLibs

clean-MyLibs:
	-$(RM) ./MyLibs/LCD_I2C.d ./MyLibs/LCD_I2C.o

.PHONY: clean-MyLibs

