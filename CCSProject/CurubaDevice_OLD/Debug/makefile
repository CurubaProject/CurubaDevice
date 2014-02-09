################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/msp430_4.2.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./Communication/network.obj" \
"./Communication/communication.obj" \
"./Control/eventManager.obj" \
"./Control/deviceInfoState.obj" \
"./Control/deviceControl.obj" \
"./Control/commsReceive.obj" \
"./Device/typeDevice.obj" \
"./Device/outlet.obj" \
"./Device/dimmer.obj" \
"./Util/util.obj" \
"./Util/commsManager.obj" \
"./Util/adcBuffer.obj" \
"./main.obj" \
"../lnk_msp430f5310.cmd" \
"../lib/CC3000 Board.lib" \
"../lib/CC3000 Spi.lib" \
"../lib/CC3000HostDriver.lib" \
"../lib/Wifi.lib" \
-l"libmath.a" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include lib/subdir_vars.mk
-include Util/subdir_vars.mk
-include Device/subdir_vars.mk
-include Control/subdir_vars.mk
-include Communication/subdir_vars.mk
-include subdir_rules.mk
-include lib/subdir_rules.mk
-include Util/subdir_rules.mk
-include Device/subdir_rules.mk
-include Control/subdir_rules.mk
-include Communication/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: CurubaDevice.out

dependents:
	-cd C:/ti/CurubaDevice/CCSProject/CC3000\ Board/Debug && $(MAKE) all
	-cd C:/ti/CurubaDevice/CCSProject/CC3000\ Spi/Debug && $(MAKE) all
	-cd C:/ti/CurubaDevice/CCSProject/CC3000HostDriver/Debug && $(MAKE) all

# Tool invocations
CurubaDevice.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS) C:/ti/CurubaDevice/CCSProject/CC3000\ Board/Debug/CC3000\ Board.lib C:/ti/CurubaDevice/CCSProject/CC3000\ Spi/Debug/CC3000\ Spi.lib C:/ti/CurubaDevice/CCSProject/CC3000HostDriver/Debug/CC3000HostDriver.lib
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=eabi -O2 --advice:power=all -g --define=__CCS__ --define=__MSP430F5310__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal -z -m"CurubaDevice.map" --heap_size=160 --stack_size=160 --use_hw_mpy=F5 -i"C:/ti/ccsv5/ccs_base/msp430/include" -i"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/lib" -i"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" -i"C:/ti/ccsv5/ccs_base/msp430/lib" --reread_libs --warn_sections --display_error_number --xml_link_info="CurubaDevice_linkInfo.xml" --rom_model -o "CurubaDevice.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "CurubaDevice.out"
	-$(RM) "main.pp" "Util\adcBuffer.pp" "Util\commsManager.pp" "Util\util.pp" "Device\dimmer.pp" "Device\outlet.pp" "Device\typeDevice.pp" "Control\commsReceive.pp" "Control\deviceControl.pp" "Control\deviceInfoState.pp" "Control\eventManager.pp" "Communication\communication.pp" "Communication\network.pp" 
	-$(RM) "main.obj" "Util\adcBuffer.obj" "Util\commsManager.obj" "Util\util.obj" "Device\dimmer.obj" "Device\outlet.obj" "Device\typeDevice.obj" "Control\commsReceive.obj" "Control\deviceControl.obj" "Control\deviceInfoState.obj" "Control\eventManager.obj" "Communication\communication.obj" "Communication\network.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/ti/CurubaDevice/CCSProject/CC3000\ Board/Debug/CC3000\ Board.lib:
C:/ti/CurubaDevice/CCSProject/CC3000\ Spi/Debug/CC3000\ Spi.lib:
C:/ti/CurubaDevice/CCSProject/CC3000HostDriver/Debug/CC3000HostDriver.lib:

-include ../makefile.targets

