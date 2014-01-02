################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Util/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Test

# Tool invocations
Test: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/TestDebug" -L"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Wifi/TestDebug" -L"/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Board/TestDebug" -Wl,-rpath=/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/CurubaDevice_OLD/TestDebug,-rpath=/home/admin02/ccs/CCS_5_5_0_00077/CCSProject/Wifi/TestDebug,-rpath="" -o "Test" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(K_DEPS)$(CLA_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Test
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
