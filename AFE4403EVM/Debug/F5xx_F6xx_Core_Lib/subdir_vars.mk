################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../F5xx_F6xx_Core_Lib/BLE.c \
../F5xx_F6xx_Core_Lib/HAL_FLASH.c \
../F5xx_F6xx_Core_Lib/HAL_I2C.c \
../F5xx_F6xx_Core_Lib/HAL_PMAP.c \
../F5xx_F6xx_Core_Lib/HAL_PMM.c \
../F5xx_F6xx_Core_Lib/HAL_TLV.c \
../F5xx_F6xx_Core_Lib/HAL_UCS.c \
../F5xx_F6xx_Core_Lib/MPU9150.c \
../F5xx_F6xx_Core_Lib/adc10_a.c \
../F5xx_F6xx_Core_Lib/adc12_a.c \
../F5xx_F6xx_Core_Lib/aes.c \
../F5xx_F6xx_Core_Lib/battbak.c \
../F5xx_F6xx_Core_Lib/comp_b.c \
../F5xx_F6xx_Core_Lib/crc.c \
../F5xx_F6xx_Core_Lib/dac12_a.c \
../F5xx_F6xx_Core_Lib/dma.c \
../F5xx_F6xx_Core_Lib/eusci_a_spi.c \
../F5xx_F6xx_Core_Lib/eusci_a_uart.c \
../F5xx_F6xx_Core_Lib/eusci_b_i2c.c \
../F5xx_F6xx_Core_Lib/eusci_b_spi.c \
../F5xx_F6xx_Core_Lib/eusci_i2c.c \
../F5xx_F6xx_Core_Lib/eusci_spi.c \
../F5xx_F6xx_Core_Lib/eusci_uart.c \
../F5xx_F6xx_Core_Lib/flashctl.c \
../F5xx_F6xx_Core_Lib/fuelGauge.c \
../F5xx_F6xx_Core_Lib/gpio.c \
../F5xx_F6xx_Core_Lib/ldopwr.c \
../F5xx_F6xx_Core_Lib/mpy32.c \
../F5xx_F6xx_Core_Lib/pmap.c \
../F5xx_F6xx_Core_Lib/pmm.c \
../F5xx_F6xx_Core_Lib/prg_info.c \
../F5xx_F6xx_Core_Lib/ram.c \
../F5xx_F6xx_Core_Lib/ref.c \
../F5xx_F6xx_Core_Lib/rtc_a.c \
../F5xx_F6xx_Core_Lib/rtc_b.c \
../F5xx_F6xx_Core_Lib/rtc_c.c \
../F5xx_F6xx_Core_Lib/sd24_b.c \
../F5xx_F6xx_Core_Lib/setMCLK.c \
../F5xx_F6xx_Core_Lib/sfr.c \
../F5xx_F6xx_Core_Lib/spansion_config_flash.c \
../F5xx_F6xx_Core_Lib/spansion_flash.c \
../F5xx_F6xx_Core_Lib/sysctl.c \
../F5xx_F6xx_Core_Lib/tec.c \
../F5xx_F6xx_Core_Lib/timer_a.c \
../F5xx_F6xx_Core_Lib/timer_b.c \
../F5xx_F6xx_Core_Lib/timer_d.c \
../F5xx_F6xx_Core_Lib/tlv.c \
../F5xx_F6xx_Core_Lib/ucs.c \
../F5xx_F6xx_Core_Lib/usci_a_spi.c \
../F5xx_F6xx_Core_Lib/usci_a_uart.c \
../F5xx_F6xx_Core_Lib/usci_b_i2c.c \
../F5xx_F6xx_Core_Lib/usci_b_spi.c \
../F5xx_F6xx_Core_Lib/usci_i2c.c \
../F5xx_F6xx_Core_Lib/usci_spi.c \
../F5xx_F6xx_Core_Lib/usci_uart.c \
../F5xx_F6xx_Core_Lib/usci_uartbaudrate.c \
../F5xx_F6xx_Core_Lib/wdt_a.c 

C_DEPS += \
./F5xx_F6xx_Core_Lib/BLE.d \
./F5xx_F6xx_Core_Lib/HAL_FLASH.d \
./F5xx_F6xx_Core_Lib/HAL_I2C.d \
./F5xx_F6xx_Core_Lib/HAL_PMAP.d \
./F5xx_F6xx_Core_Lib/HAL_PMM.d \
./F5xx_F6xx_Core_Lib/HAL_TLV.d \
./F5xx_F6xx_Core_Lib/HAL_UCS.d \
./F5xx_F6xx_Core_Lib/MPU9150.d \
./F5xx_F6xx_Core_Lib/adc10_a.d \
./F5xx_F6xx_Core_Lib/adc12_a.d \
./F5xx_F6xx_Core_Lib/aes.d \
./F5xx_F6xx_Core_Lib/battbak.d \
./F5xx_F6xx_Core_Lib/comp_b.d \
./F5xx_F6xx_Core_Lib/crc.d \
./F5xx_F6xx_Core_Lib/dac12_a.d \
./F5xx_F6xx_Core_Lib/dma.d \
./F5xx_F6xx_Core_Lib/eusci_a_spi.d \
./F5xx_F6xx_Core_Lib/eusci_a_uart.d \
./F5xx_F6xx_Core_Lib/eusci_b_i2c.d \
./F5xx_F6xx_Core_Lib/eusci_b_spi.d \
./F5xx_F6xx_Core_Lib/eusci_i2c.d \
./F5xx_F6xx_Core_Lib/eusci_spi.d \
./F5xx_F6xx_Core_Lib/eusci_uart.d \
./F5xx_F6xx_Core_Lib/flashctl.d \
./F5xx_F6xx_Core_Lib/fuelGauge.d \
./F5xx_F6xx_Core_Lib/gpio.d \
./F5xx_F6xx_Core_Lib/ldopwr.d \
./F5xx_F6xx_Core_Lib/mpy32.d \
./F5xx_F6xx_Core_Lib/pmap.d \
./F5xx_F6xx_Core_Lib/pmm.d \
./F5xx_F6xx_Core_Lib/prg_info.d \
./F5xx_F6xx_Core_Lib/ram.d \
./F5xx_F6xx_Core_Lib/ref.d \
./F5xx_F6xx_Core_Lib/rtc_a.d \
./F5xx_F6xx_Core_Lib/rtc_b.d \
./F5xx_F6xx_Core_Lib/rtc_c.d \
./F5xx_F6xx_Core_Lib/sd24_b.d \
./F5xx_F6xx_Core_Lib/setMCLK.d \
./F5xx_F6xx_Core_Lib/sfr.d \
./F5xx_F6xx_Core_Lib/spansion_config_flash.d \
./F5xx_F6xx_Core_Lib/spansion_flash.d \
./F5xx_F6xx_Core_Lib/sysctl.d \
./F5xx_F6xx_Core_Lib/tec.d \
./F5xx_F6xx_Core_Lib/timer_a.d \
./F5xx_F6xx_Core_Lib/timer_b.d \
./F5xx_F6xx_Core_Lib/timer_d.d \
./F5xx_F6xx_Core_Lib/tlv.d \
./F5xx_F6xx_Core_Lib/ucs.d \
./F5xx_F6xx_Core_Lib/usci_a_spi.d \
./F5xx_F6xx_Core_Lib/usci_a_uart.d \
./F5xx_F6xx_Core_Lib/usci_b_i2c.d \
./F5xx_F6xx_Core_Lib/usci_b_spi.d \
./F5xx_F6xx_Core_Lib/usci_i2c.d \
./F5xx_F6xx_Core_Lib/usci_spi.d \
./F5xx_F6xx_Core_Lib/usci_uart.d \
./F5xx_F6xx_Core_Lib/usci_uartbaudrate.d \
./F5xx_F6xx_Core_Lib/wdt_a.d 

OBJS += \
./F5xx_F6xx_Core_Lib/BLE.obj \
./F5xx_F6xx_Core_Lib/HAL_FLASH.obj \
./F5xx_F6xx_Core_Lib/HAL_I2C.obj \
./F5xx_F6xx_Core_Lib/HAL_PMAP.obj \
./F5xx_F6xx_Core_Lib/HAL_PMM.obj \
./F5xx_F6xx_Core_Lib/HAL_TLV.obj \
./F5xx_F6xx_Core_Lib/HAL_UCS.obj \
./F5xx_F6xx_Core_Lib/MPU9150.obj \
./F5xx_F6xx_Core_Lib/adc10_a.obj \
./F5xx_F6xx_Core_Lib/adc12_a.obj \
./F5xx_F6xx_Core_Lib/aes.obj \
./F5xx_F6xx_Core_Lib/battbak.obj \
./F5xx_F6xx_Core_Lib/comp_b.obj \
./F5xx_F6xx_Core_Lib/crc.obj \
./F5xx_F6xx_Core_Lib/dac12_a.obj \
./F5xx_F6xx_Core_Lib/dma.obj \
./F5xx_F6xx_Core_Lib/eusci_a_spi.obj \
./F5xx_F6xx_Core_Lib/eusci_a_uart.obj \
./F5xx_F6xx_Core_Lib/eusci_b_i2c.obj \
./F5xx_F6xx_Core_Lib/eusci_b_spi.obj \
./F5xx_F6xx_Core_Lib/eusci_i2c.obj \
./F5xx_F6xx_Core_Lib/eusci_spi.obj \
./F5xx_F6xx_Core_Lib/eusci_uart.obj \
./F5xx_F6xx_Core_Lib/flashctl.obj \
./F5xx_F6xx_Core_Lib/fuelGauge.obj \
./F5xx_F6xx_Core_Lib/gpio.obj \
./F5xx_F6xx_Core_Lib/ldopwr.obj \
./F5xx_F6xx_Core_Lib/mpy32.obj \
./F5xx_F6xx_Core_Lib/pmap.obj \
./F5xx_F6xx_Core_Lib/pmm.obj \
./F5xx_F6xx_Core_Lib/prg_info.obj \
./F5xx_F6xx_Core_Lib/ram.obj \
./F5xx_F6xx_Core_Lib/ref.obj \
./F5xx_F6xx_Core_Lib/rtc_a.obj \
./F5xx_F6xx_Core_Lib/rtc_b.obj \
./F5xx_F6xx_Core_Lib/rtc_c.obj \
./F5xx_F6xx_Core_Lib/sd24_b.obj \
./F5xx_F6xx_Core_Lib/setMCLK.obj \
./F5xx_F6xx_Core_Lib/sfr.obj \
./F5xx_F6xx_Core_Lib/spansion_config_flash.obj \
./F5xx_F6xx_Core_Lib/spansion_flash.obj \
./F5xx_F6xx_Core_Lib/sysctl.obj \
./F5xx_F6xx_Core_Lib/tec.obj \
./F5xx_F6xx_Core_Lib/timer_a.obj \
./F5xx_F6xx_Core_Lib/timer_b.obj \
./F5xx_F6xx_Core_Lib/timer_d.obj \
./F5xx_F6xx_Core_Lib/tlv.obj \
./F5xx_F6xx_Core_Lib/ucs.obj \
./F5xx_F6xx_Core_Lib/usci_a_spi.obj \
./F5xx_F6xx_Core_Lib/usci_a_uart.obj \
./F5xx_F6xx_Core_Lib/usci_b_i2c.obj \
./F5xx_F6xx_Core_Lib/usci_b_spi.obj \
./F5xx_F6xx_Core_Lib/usci_i2c.obj \
./F5xx_F6xx_Core_Lib/usci_spi.obj \
./F5xx_F6xx_Core_Lib/usci_uart.obj \
./F5xx_F6xx_Core_Lib/usci_uartbaudrate.obj \
./F5xx_F6xx_Core_Lib/wdt_a.obj 

OBJS__QUOTED += \
"F5xx_F6xx_Core_Lib\BLE.obj" \
"F5xx_F6xx_Core_Lib\HAL_FLASH.obj" \
"F5xx_F6xx_Core_Lib\HAL_I2C.obj" \
"F5xx_F6xx_Core_Lib\HAL_PMAP.obj" \
"F5xx_F6xx_Core_Lib\HAL_PMM.obj" \
"F5xx_F6xx_Core_Lib\HAL_TLV.obj" \
"F5xx_F6xx_Core_Lib\HAL_UCS.obj" \
"F5xx_F6xx_Core_Lib\MPU9150.obj" \
"F5xx_F6xx_Core_Lib\adc10_a.obj" \
"F5xx_F6xx_Core_Lib\adc12_a.obj" \
"F5xx_F6xx_Core_Lib\aes.obj" \
"F5xx_F6xx_Core_Lib\battbak.obj" \
"F5xx_F6xx_Core_Lib\comp_b.obj" \
"F5xx_F6xx_Core_Lib\crc.obj" \
"F5xx_F6xx_Core_Lib\dac12_a.obj" \
"F5xx_F6xx_Core_Lib\dma.obj" \
"F5xx_F6xx_Core_Lib\eusci_a_spi.obj" \
"F5xx_F6xx_Core_Lib\eusci_a_uart.obj" \
"F5xx_F6xx_Core_Lib\eusci_b_i2c.obj" \
"F5xx_F6xx_Core_Lib\eusci_b_spi.obj" \
"F5xx_F6xx_Core_Lib\eusci_i2c.obj" \
"F5xx_F6xx_Core_Lib\eusci_spi.obj" \
"F5xx_F6xx_Core_Lib\eusci_uart.obj" \
"F5xx_F6xx_Core_Lib\flashctl.obj" \
"F5xx_F6xx_Core_Lib\fuelGauge.obj" \
"F5xx_F6xx_Core_Lib\gpio.obj" \
"F5xx_F6xx_Core_Lib\ldopwr.obj" \
"F5xx_F6xx_Core_Lib\mpy32.obj" \
"F5xx_F6xx_Core_Lib\pmap.obj" \
"F5xx_F6xx_Core_Lib\pmm.obj" \
"F5xx_F6xx_Core_Lib\prg_info.obj" \
"F5xx_F6xx_Core_Lib\ram.obj" \
"F5xx_F6xx_Core_Lib\ref.obj" \
"F5xx_F6xx_Core_Lib\rtc_a.obj" \
"F5xx_F6xx_Core_Lib\rtc_b.obj" \
"F5xx_F6xx_Core_Lib\rtc_c.obj" \
"F5xx_F6xx_Core_Lib\sd24_b.obj" \
"F5xx_F6xx_Core_Lib\setMCLK.obj" \
"F5xx_F6xx_Core_Lib\sfr.obj" \
"F5xx_F6xx_Core_Lib\spansion_config_flash.obj" \
"F5xx_F6xx_Core_Lib\spansion_flash.obj" \
"F5xx_F6xx_Core_Lib\sysctl.obj" \
"F5xx_F6xx_Core_Lib\tec.obj" \
"F5xx_F6xx_Core_Lib\timer_a.obj" \
"F5xx_F6xx_Core_Lib\timer_b.obj" \
"F5xx_F6xx_Core_Lib\timer_d.obj" \
"F5xx_F6xx_Core_Lib\tlv.obj" \
"F5xx_F6xx_Core_Lib\ucs.obj" \
"F5xx_F6xx_Core_Lib\usci_a_spi.obj" \
"F5xx_F6xx_Core_Lib\usci_a_uart.obj" \
"F5xx_F6xx_Core_Lib\usci_b_i2c.obj" \
"F5xx_F6xx_Core_Lib\usci_b_spi.obj" \
"F5xx_F6xx_Core_Lib\usci_i2c.obj" \
"F5xx_F6xx_Core_Lib\usci_spi.obj" \
"F5xx_F6xx_Core_Lib\usci_uart.obj" \
"F5xx_F6xx_Core_Lib\usci_uartbaudrate.obj" \
"F5xx_F6xx_Core_Lib\wdt_a.obj" 

C_DEPS__QUOTED += \
"F5xx_F6xx_Core_Lib\BLE.d" \
"F5xx_F6xx_Core_Lib\HAL_FLASH.d" \
"F5xx_F6xx_Core_Lib\HAL_I2C.d" \
"F5xx_F6xx_Core_Lib\HAL_PMAP.d" \
"F5xx_F6xx_Core_Lib\HAL_PMM.d" \
"F5xx_F6xx_Core_Lib\HAL_TLV.d" \
"F5xx_F6xx_Core_Lib\HAL_UCS.d" \
"F5xx_F6xx_Core_Lib\MPU9150.d" \
"F5xx_F6xx_Core_Lib\adc10_a.d" \
"F5xx_F6xx_Core_Lib\adc12_a.d" \
"F5xx_F6xx_Core_Lib\aes.d" \
"F5xx_F6xx_Core_Lib\battbak.d" \
"F5xx_F6xx_Core_Lib\comp_b.d" \
"F5xx_F6xx_Core_Lib\crc.d" \
"F5xx_F6xx_Core_Lib\dac12_a.d" \
"F5xx_F6xx_Core_Lib\dma.d" \
"F5xx_F6xx_Core_Lib\eusci_a_spi.d" \
"F5xx_F6xx_Core_Lib\eusci_a_uart.d" \
"F5xx_F6xx_Core_Lib\eusci_b_i2c.d" \
"F5xx_F6xx_Core_Lib\eusci_b_spi.d" \
"F5xx_F6xx_Core_Lib\eusci_i2c.d" \
"F5xx_F6xx_Core_Lib\eusci_spi.d" \
"F5xx_F6xx_Core_Lib\eusci_uart.d" \
"F5xx_F6xx_Core_Lib\flashctl.d" \
"F5xx_F6xx_Core_Lib\fuelGauge.d" \
"F5xx_F6xx_Core_Lib\gpio.d" \
"F5xx_F6xx_Core_Lib\ldopwr.d" \
"F5xx_F6xx_Core_Lib\mpy32.d" \
"F5xx_F6xx_Core_Lib\pmap.d" \
"F5xx_F6xx_Core_Lib\pmm.d" \
"F5xx_F6xx_Core_Lib\prg_info.d" \
"F5xx_F6xx_Core_Lib\ram.d" \
"F5xx_F6xx_Core_Lib\ref.d" \
"F5xx_F6xx_Core_Lib\rtc_a.d" \
"F5xx_F6xx_Core_Lib\rtc_b.d" \
"F5xx_F6xx_Core_Lib\rtc_c.d" \
"F5xx_F6xx_Core_Lib\sd24_b.d" \
"F5xx_F6xx_Core_Lib\setMCLK.d" \
"F5xx_F6xx_Core_Lib\sfr.d" \
"F5xx_F6xx_Core_Lib\spansion_config_flash.d" \
"F5xx_F6xx_Core_Lib\spansion_flash.d" \
"F5xx_F6xx_Core_Lib\sysctl.d" \
"F5xx_F6xx_Core_Lib\tec.d" \
"F5xx_F6xx_Core_Lib\timer_a.d" \
"F5xx_F6xx_Core_Lib\timer_b.d" \
"F5xx_F6xx_Core_Lib\timer_d.d" \
"F5xx_F6xx_Core_Lib\tlv.d" \
"F5xx_F6xx_Core_Lib\ucs.d" \
"F5xx_F6xx_Core_Lib\usci_a_spi.d" \
"F5xx_F6xx_Core_Lib\usci_a_uart.d" \
"F5xx_F6xx_Core_Lib\usci_b_i2c.d" \
"F5xx_F6xx_Core_Lib\usci_b_spi.d" \
"F5xx_F6xx_Core_Lib\usci_i2c.d" \
"F5xx_F6xx_Core_Lib\usci_spi.d" \
"F5xx_F6xx_Core_Lib\usci_uart.d" \
"F5xx_F6xx_Core_Lib\usci_uartbaudrate.d" \
"F5xx_F6xx_Core_Lib\wdt_a.d" 

C_SRCS__QUOTED += \
"../F5xx_F6xx_Core_Lib/BLE.c" \
"../F5xx_F6xx_Core_Lib/HAL_FLASH.c" \
"../F5xx_F6xx_Core_Lib/HAL_I2C.c" \
"../F5xx_F6xx_Core_Lib/HAL_PMAP.c" \
"../F5xx_F6xx_Core_Lib/HAL_PMM.c" \
"../F5xx_F6xx_Core_Lib/HAL_TLV.c" \
"../F5xx_F6xx_Core_Lib/HAL_UCS.c" \
"../F5xx_F6xx_Core_Lib/MPU9150.c" \
"../F5xx_F6xx_Core_Lib/adc10_a.c" \
"../F5xx_F6xx_Core_Lib/adc12_a.c" \
"../F5xx_F6xx_Core_Lib/aes.c" \
"../F5xx_F6xx_Core_Lib/battbak.c" \
"../F5xx_F6xx_Core_Lib/comp_b.c" \
"../F5xx_F6xx_Core_Lib/crc.c" \
"../F5xx_F6xx_Core_Lib/dac12_a.c" \
"../F5xx_F6xx_Core_Lib/dma.c" \
"../F5xx_F6xx_Core_Lib/eusci_a_spi.c" \
"../F5xx_F6xx_Core_Lib/eusci_a_uart.c" \
"../F5xx_F6xx_Core_Lib/eusci_b_i2c.c" \
"../F5xx_F6xx_Core_Lib/eusci_b_spi.c" \
"../F5xx_F6xx_Core_Lib/eusci_i2c.c" \
"../F5xx_F6xx_Core_Lib/eusci_spi.c" \
"../F5xx_F6xx_Core_Lib/eusci_uart.c" \
"../F5xx_F6xx_Core_Lib/flashctl.c" \
"../F5xx_F6xx_Core_Lib/fuelGauge.c" \
"../F5xx_F6xx_Core_Lib/gpio.c" \
"../F5xx_F6xx_Core_Lib/ldopwr.c" \
"../F5xx_F6xx_Core_Lib/mpy32.c" \
"../F5xx_F6xx_Core_Lib/pmap.c" \
"../F5xx_F6xx_Core_Lib/pmm.c" \
"../F5xx_F6xx_Core_Lib/prg_info.c" \
"../F5xx_F6xx_Core_Lib/ram.c" \
"../F5xx_F6xx_Core_Lib/ref.c" \
"../F5xx_F6xx_Core_Lib/rtc_a.c" \
"../F5xx_F6xx_Core_Lib/rtc_b.c" \
"../F5xx_F6xx_Core_Lib/rtc_c.c" \
"../F5xx_F6xx_Core_Lib/sd24_b.c" \
"../F5xx_F6xx_Core_Lib/setMCLK.c" \
"../F5xx_F6xx_Core_Lib/sfr.c" \
"../F5xx_F6xx_Core_Lib/spansion_config_flash.c" \
"../F5xx_F6xx_Core_Lib/spansion_flash.c" \
"../F5xx_F6xx_Core_Lib/sysctl.c" \
"../F5xx_F6xx_Core_Lib/tec.c" \
"../F5xx_F6xx_Core_Lib/timer_a.c" \
"../F5xx_F6xx_Core_Lib/timer_b.c" \
"../F5xx_F6xx_Core_Lib/timer_d.c" \
"../F5xx_F6xx_Core_Lib/tlv.c" \
"../F5xx_F6xx_Core_Lib/ucs.c" \
"../F5xx_F6xx_Core_Lib/usci_a_spi.c" \
"../F5xx_F6xx_Core_Lib/usci_a_uart.c" \
"../F5xx_F6xx_Core_Lib/usci_b_i2c.c" \
"../F5xx_F6xx_Core_Lib/usci_b_spi.c" \
"../F5xx_F6xx_Core_Lib/usci_i2c.c" \
"../F5xx_F6xx_Core_Lib/usci_spi.c" \
"../F5xx_F6xx_Core_Lib/usci_uart.c" \
"../F5xx_F6xx_Core_Lib/usci_uartbaudrate.c" \
"../F5xx_F6xx_Core_Lib/wdt_a.c" 


