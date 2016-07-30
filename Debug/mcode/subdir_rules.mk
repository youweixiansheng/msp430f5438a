################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
mcode/gene.obj: ../mcode/gene.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"E:/TI/ccsv6/tools/compiler/ti-cgt-msp430_4.4.5/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="E:/TI/ccsv6/ccs_base/msp430/include" --include_path="H:/msp430/lpwMSP430/driverlib/MSP430F5xx_6xx" --include_path="E:/TI/ccsv6/tools/compiler/ti-cgt-msp430_4.4.5/include" -g --define=DEPRECATED --define=__MSP430F5438A__ --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="mcode/gene.pp" --obj_directory="mcode" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


