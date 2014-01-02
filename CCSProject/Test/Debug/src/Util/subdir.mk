################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/admin02/ccs/CCS_5_5_0_00077/Source/Test/testCommsManager.cpp 

OBJS += \
./src/Util/testCommsManager.o 

CPP_DEPS += \
./src/Util/testCommsManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/Util/testCommsManager.o: /home/admin02/ccs/CCS_5_5_0_00077/Source/Test/testCommsManager.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Test/cute" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Test/../../Source/CurubaDevice" -I"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Test/../../Source/Test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


