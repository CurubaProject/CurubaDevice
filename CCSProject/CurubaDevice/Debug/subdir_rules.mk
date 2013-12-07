################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CommsManager.obj: C:/ti/Curuba/Source/CurubaDevice/CommsManager.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/Curuba/Source/CC3000HostDriver" --include_path="C:/ti/Curuba/Source/CC3000 Spi" --include_path="C:/ti/Curuba/Source/CC3000 Board" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power="all" --define=__MSP430F5310__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="CommsManager.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

domumapp.obj: C:/ti/Curuba/Source/CurubaDevice/domumapp.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/Curuba/Source/CC3000HostDriver" --include_path="C:/ti/Curuba/Source/CC3000 Spi" --include_path="C:/ti/Curuba/Source/CC3000 Board" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power="all" --define=__MSP430F5310__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="domumapp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: C:/ti/Curuba/Source/CurubaDevice/main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/Curuba/Source/CC3000HostDriver" --include_path="C:/ti/Curuba/Source/CC3000 Spi" --include_path="C:/ti/Curuba/Source/CC3000 Board" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power="all" --define=__MSP430F5310__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

wifi.obj: C:/ti/Curuba/Source/CurubaDevice/wifi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmspx --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/Curuba/Source/CC3000HostDriver" --include_path="C:/ti/Curuba/Source/CC3000 Spi" --include_path="C:/ti/Curuba/Source/CC3000 Board" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power="all" --define=__MSP430F5310__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="wifi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


