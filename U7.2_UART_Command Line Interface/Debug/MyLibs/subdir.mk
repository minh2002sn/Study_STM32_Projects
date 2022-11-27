################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyLibs/cli_command.c \
../MyLibs/cli_command_table.c \
../MyLibs/led_app.c \
../MyLibs/led_driver.c \
../MyLibs/read_time.c \
../MyLibs/response.c \
../MyLibs/uart.c 

OBJS += \
./MyLibs/cli_command.o \
./MyLibs/cli_command_table.o \
./MyLibs/led_app.o \
./MyLibs/led_driver.o \
./MyLibs/read_time.o \
./MyLibs/response.o \
./MyLibs/uart.o 

C_DEPS += \
./MyLibs/cli_command.d \
./MyLibs/cli_command_table.d \
./MyLibs/led_app.d \
./MyLibs/led_driver.d \
./MyLibs/read_time.d \
./MyLibs/response.d \
./MyLibs/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MyLibs/%.o: ../MyLibs/%.c MyLibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/A_Minh/B_STM32_Projects/StudySTM32/U7.2_UART_Command Line Interface/MyLibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyLibs

clean-MyLibs:
	-$(RM) ./MyLibs/cli_command.d ./MyLibs/cli_command.o ./MyLibs/cli_command_table.d ./MyLibs/cli_command_table.o ./MyLibs/led_app.d ./MyLibs/led_app.o ./MyLibs/led_driver.d ./MyLibs/led_driver.o ./MyLibs/read_time.d ./MyLibs/read_time.o ./MyLibs/response.d ./MyLibs/response.o ./MyLibs/uart.d ./MyLibs/uart.o

.PHONY: clean-MyLibs

