################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/commsReceive.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/deviceControl.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/deviceInfoState.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/eventManager.c 

OBJS += \
./src/Control/commsReceive.o \
./src/Control/deviceControl.o \
./src/Control/deviceInfoState.o \
./src/Control/eventManager.o 

C_DEPS += \
./src/Control/commsReceive.d \
./src/Control/deviceControl.d \
./src/Control/deviceInfoState.d \
./src/Control/eventManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/Control/commsReceive.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/commsReceive.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Control/deviceControl.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/deviceControl.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Control/deviceInfoState.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/deviceInfoState.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Control/eventManager.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/eventManager.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


