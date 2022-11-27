################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyLibs/bootloader.c \
../MyLibs/bootloader_command.c \
../MyLibs/min.c \
../MyLibs/port_min.c \
../MyLibs/ring_buffer.c \
../MyLibs/uart.c 

OBJS += \
./MyLibs/bootloader.o \
./MyLibs/bootloader_command.o \
./MyLibs/min.o \
./MyLibs/port_min.o \
./MyLibs/ring_buffer.o \
./MyLibs/uart.o 

C_DEPS += \
./MyLibs/bootloader.d \
./MyLibs/bootloader_command.d \
./MyLibs/min.d \
./MyLibs/port_min.d \
./MyLibs/ring_buffer.d \
./MyLibs/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MyLibs/%.o: ../MyLibs/%.c MyLibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/A_Minh/B_STM32_Projects/StudySTM32/U12.1_Update_OTA/MyLibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyLibs

clean-MyLibs:
	-$(RM) ./MyLibs/bootloader.d ./MyLibs/bootloader.o ./MyLibs/bootloader_command.d ./MyLibs/bootloader_command.o ./MyLibs/min.d ./MyLibs/min.o ./MyLibs/port_min.d ./MyLibs/port_min.o ./MyLibs/ring_buffer.d ./MyLibs/ring_buffer.o ./MyLibs/uart.d ./MyLibs/uart.o

.PHONY: clean-MyLibs

