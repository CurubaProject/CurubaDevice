################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/adcBuffer.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/commsManager.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/util.c 

OBJS += \
./src/Util/adcBuffer.o \
./src/Util/commsManager.o \
./src/Util/util.o 

C_DEPS += \
./src/Util/adcBuffer.d \
./src/Util/commsManager.d \
./src/Util/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/Util/adcBuffer.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/adcBuffer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Util/commsManager.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/commsManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Util/util.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/util.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


