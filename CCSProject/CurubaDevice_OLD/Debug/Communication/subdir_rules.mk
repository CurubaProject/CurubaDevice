################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Communication/communication.obj: C:/ti/CurubaDevice/Source/CurubaDevice/communication.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=eabi -O2 --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/CurubaDevice/Source/CC3000 Board" --include_path="C:/ti/CurubaDevice/Source/Wifi" --include_path="C:/ti/CurubaDevice/Source/CC3000 Spi" --include_path="C:/ti/CurubaDevice/Source/CC3000HostDriver" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__CCS__ --define=__MSP430F5310__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Communication/communication.pp" --obj_directory="Communication" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Communication/network.obj: C:/ti/CurubaDevice/Source/CurubaDevice/network.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=eabi -O2 --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/CurubaDevice/Source/CC3000 Board" --include_path="C:/ti/CurubaDevice/Source/Wifi" --include_path="C:/ti/CurubaDevice/Source/CC3000 Spi" --include_path="C:/ti/CurubaDevice/Source/CC3000HostDriver" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power=all -g --define=__CCS__ --define=__MSP430F5310__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="Communication/network.pp" --obj_directory="Communication" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


