################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F5xx_F6xx_Core_Lib/%.obj: ../F5xx_F6xx_Core_Lib/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.3.LTS/bin/cl430" -vmspx --abi=eabi --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.3.LTS/include" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/F5xx_F6xx_Core_Lib/deprecated" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/F5xx_F6xx_Core_Lib/inc" --include_path="C:/ti/ccsv6/ccs_base/msp430/lib" --include_path="C:/ti/ccsv6/ccs_base/msp430/lib/5xx_6xx_FRxx" --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/AFE44x0_MSP430F5528_APP" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/F5xx_F6xx_Core_Lib" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API/USB_CDC_API" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API/USB_Common" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API/USB_HID_API" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API/USB_MSC_API" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_API/USB_PHDC_API" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_User" --include_path="C:/Users/xiaos/workspace_v8_1/CES_HRMWatch_PC_EVM_GUI_v2.0-CCS/USB/USB_config" --advice:power="all" -g --define=__MSP430F5528__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="F5xx_F6xx_Core_Lib/$(basename $(<F)).d_raw" --obj_directory="F5xx_F6xx_Core_Lib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


