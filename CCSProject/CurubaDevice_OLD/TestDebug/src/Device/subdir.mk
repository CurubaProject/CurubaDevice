################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/dimmer.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/outlet.c \
/home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/typeDevice.c 

OBJS += \
./src/Device/dimmer.o \
./src/Device/outlet.o \
./src/Device/typeDevice.o 

C_DEPS += \
./src/Device/dimmer.d \
./src/Device/outlet.d \
./src/Device/typeDevice.d 


# Each subdirectory must supply rules for building sources it contributes
src/Device/dimmer.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/dimmer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Device/outlet.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/outlet.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Device/typeDevice.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/CurubaDevice/typeDevice.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/CC3000 Board" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/../../Source/Wifi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


