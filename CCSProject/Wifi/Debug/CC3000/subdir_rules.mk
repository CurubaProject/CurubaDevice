################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CC3000/cc3000.obj: /home/admin02/ccs/CCS_5_5_0_00077/Source/Wifi/cc3000.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/home/admin02/ccs/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=eabi --include_path="/home/admin02/ccs/ccsv5/ccs_base/msp430/include" --include_path="/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000HostDriver" --include_path="/home/admin02/ccs/CCS_5_5_0_00077/Source/CC3000 Spi" --include_path="/home/admin02/ccs/CCS_5_5_0_00077/Source/Board" --include_path="/home/admin02/ccs/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__MSP430F5310__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="CC3000/cc3000.pp" --obj_directory="CC3000" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


