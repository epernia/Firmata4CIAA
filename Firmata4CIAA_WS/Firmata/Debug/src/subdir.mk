################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FIRMATA.c \
../src/HAL.c \
../src/SERVO_DRIVER.c \
../src/TIMER_DRIVER.c \
../src/UART_DRIVER.c \
../src/main.c 

OBJS += \
./src/FIRMATA.o \
./src/HAL.o \
./src/SERVO_DRIVER.o \
./src/TIMER_DRIVER.o \
./src/UART_DRIVER.o \
./src/main.o 

C_DEPS += \
./src/FIRMATA.d \
./src/HAL.d \
./src/SERVO_DRIVER.d \
./src/TIMER_DRIVER.d \
./src/UART_DRIVER.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__CODE_RED -D__LPC43XX__ -D__REDLIB__ -D__USE_LPCOPEN -DCORE_M4 -DDEBUG -I"C:\Users\Ian\Desktop\Firmata4CIAA_WS\Firmata\inc" -I"C:\Users\Ian\Desktop\Firmata4CIAA_WS\freertos_lpc43xx\inc" -I"C:\Users\Ian\Desktop\Firmata4CIAA_WS\lpcopen_lpc4337\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


