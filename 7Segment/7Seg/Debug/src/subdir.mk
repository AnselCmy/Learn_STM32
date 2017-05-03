################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/7Seg.c \
../src/gpio.c \
../src/main.c 

OBJS += \
./src/7Seg.o \
./src/gpio.o \
./src/main.o 

C_DEPS += \
./src/7Seg.d \
./src/gpio.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DSTM32 -DDEBUG -I"/Users/chen/Documents/curriculum/microprossor/7Segment/7Seg/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


