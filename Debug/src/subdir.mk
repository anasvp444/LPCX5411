################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc5411x.c \
../src/crp.c \
../src/main.c \
../src/sysinit.c 

OBJS += \
./src/cr_startup_lpc5411x.o \
./src/crp.o \
./src/main.o \
./src/sysinit.o 

C_DEPS += \
./src/cr_startup_lpc5411x.d \
./src/crp.d \
./src/main.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__MULTICORE_NONE -D__USE_LPCOPEN -DCORE_M4 -D__CODE_RED -D__LPC5411X__ -D__REDLIB__ -DDEBUG -I"D:\anas_datas\workspace1\lpc_chip_5411x\inc" -I"D:\anas_datas\workspace1\lpc_board_lpcxpresso_54114\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


