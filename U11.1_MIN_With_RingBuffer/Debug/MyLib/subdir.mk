################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyLib/min.c \
../MyLib/ring_buffer.c \
../MyLib/uart.c 

OBJS += \
./MyLib/min.o \
./MyLib/ring_buffer.o \
./MyLib/uart.o 

C_DEPS += \
./MyLib/min.d \
./MyLib/ring_buffer.d \
./MyLib/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MyLib/%.o: ../MyLib/%.c MyLib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/A_Minh/B_STM32_Projects/StudySTM32/U11.1_MIN_With_RingBuffer/MyLib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyLib

clean-MyLib:
	-$(RM) ./MyLib/min.d ./MyLib/min.o ./MyLib/ring_buffer.d ./MyLib/ring_buffer.o ./MyLib/uart.d ./MyLib/uart.o

.PHONY: clean-MyLib

