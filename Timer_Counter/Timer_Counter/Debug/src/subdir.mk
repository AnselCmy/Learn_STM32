################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/7seg.c \
../src/gpio.c \
../src/keypad.c \
../src/main.c \
../src/timer_counter.c 

OBJS += \
./src/7seg.o \
./src/gpio.o \
./src/keypad.o \
./src/main.o \
./src/timer_counter.o 

C_DEPS += \
./src/7seg.d \
./src/gpio.d \
./src/keypad.d \
./src/main.d \
./src/timer_counter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DSTM32 -DDEBUG -I"/Users/chen/code/STM32_workplace/Timer_Counter/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


