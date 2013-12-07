################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
spi.obj: C:/ti/CurubaDevice/Source/CC3000\ Spi/spi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.8/bin/cl430" -vmspx --abi=eabi -O2 --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/CurubaDevice/Source/CC3000HostDriver" --include_path="C:/ti/CurubaDevice/Source/CC3000 Board" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.8/include" --advice:power=all --define=__MSP430F5529__ --define=__CCS__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="spi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


