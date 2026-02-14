################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/EcoLib/ecocan/ecocar_can.c 

OBJS += \
./lib/EcoLib/ecocan/ecocar_can.o 

C_DEPS += \
./lib/EcoLib/ecocan/ecocar_can.d 


# Each subdirectory must supply rules for building sources it contributes
lib/EcoLib/ecocan/%.o lib/EcoLib/ecocan/%.su lib/EcoLib/ecocan/%.cyclo: ../lib/EcoLib/ecocan/%.c lib/EcoLib/ecocan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/ssd1306/inc" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/bme280" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util/log" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/ecocan" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/MQ8" -I"F:/EcoCar/H2-Sensor-Sally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/EEPROM-Emulator/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-EcoLib-2f-ecocan

clean-lib-2f-EcoLib-2f-ecocan:
	-$(RM) ./lib/EcoLib/ecocan/ecocar_can.cyclo ./lib/EcoLib/ecocan/ecocar_can.d ./lib/EcoLib/ecocan/ecocar_can.o ./lib/EcoLib/ecocan/ecocar_can.su

.PHONY: clean-lib-2f-EcoLib-2f-ecocan

