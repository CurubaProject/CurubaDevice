################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/boardSimulator.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/interruptSimulator.c 

OBJS += \
./Simulator/boardSimulator.o \
./Simulator/interruptSimulator.o 

C_DEPS += \
./Simulator/boardSimulator.d \
./Simulator/interruptSimulator.d 


# Each subdirectory must supply rules for building sources it contributes
Simulator/boardSimulator.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/boardSimulator.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Simulator/boardSimulator.d" -MT"Simulator/boardSimulator.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Simulator/interruptSimulator.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000\ Board/interruptSimulator.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__TESTDEBUG__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Simulator/interruptSimulator.d" -MT"Simulator/interruptSimulator.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


