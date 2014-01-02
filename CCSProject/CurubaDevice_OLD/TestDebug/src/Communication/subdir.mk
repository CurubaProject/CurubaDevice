################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/communication.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/network.c 

OBJS += \
./src/Communication/communication.o \
./src/Communication/network.o 

C_DEPS += \
./src/Communication/communication.d \
./src/Communication/network.d 


# Each subdirectory must supply rules for building sources it contributes
src/Communication/communication.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/communication.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Communication/network.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/network.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


