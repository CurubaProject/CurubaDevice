################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/boardMSP430.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/interuptDeviceControl.c 

OBJS += \
./MSP430F5310/boardMSP430.o \
./MSP430F5310/interuptDeviceControl.o 

C_DEPS += \
./MSP430F5310/boardMSP430.d \
./MSP430F5310/interuptDeviceControl.d 


# Each subdirectory must supply rules for building sources it contributes
MSP430F5310/boardMSP430.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/boardMSP430.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"MSP430F5310/boardMSP430.d" -MT"MSP430F5310/boardMSP430.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

MSP430F5310/interuptDeviceControl.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/interuptDeviceControl.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"MSP430F5310/interuptDeviceControl.d" -MT"MSP430F5310/interuptDeviceControl.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


