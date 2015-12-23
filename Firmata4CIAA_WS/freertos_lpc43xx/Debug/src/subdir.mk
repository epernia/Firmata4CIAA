################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOSCommonHooks.c \
../src/croutine.c \
../src/event_groups.c \
../src/heap_2.c \
../src/list.c \
../src/port.c \
../src/queue.c \
../src/tasks.c \
../src/timers.c 

OBJS += \
./src/FreeRTOSCommonHooks.o \
./src/croutine.o \
./src/event_groups.o \
./src/heap_2.o \
./src/list.o \
./src/port.o \
./src/queue.o \
./src/tasks.o \
./src/timers.o 

C_DEPS += \
./src/FreeRTOSCommonHooks.d \
./src/croutine.d \
./src/event_groups.d \
./src/heap_2.d \
./src/list.d \
./src/port.d \
./src/queue.d \
./src/tasks.d \
./src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__CODE_RED -D__LPC43XX__ -D__USE_LPCOPEN -DCORE_M4 -I"C:\Users\Ian\Desktop\Firmata4CIAA_WS\lpcopen_lpc4337\inc" -I"C:\Users\Ian\Desktop\Firmata4CIAA_WS\freertos_lpc43xx\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


