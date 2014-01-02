################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/Wifi/wifiSimulator.c 

OBJS += \
./Simulator/wifiSimulator.o 

C_DEPS += \
./Simulator/wifiSimulator.d 


# Each subdirectory must supply rules for building sources it contributes
Simulator/wifiSimulator.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/Wifi/wifiSimulator.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


