################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/Wifi/cc3000.c 

OBJS += \
./CC3000/cc3000.o 

C_DEPS += \
./CC3000/cc3000.d 


# Each subdirectory must supply rules for building sources it contributes
CC3000/cc3000.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/Wifi/cc3000.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


