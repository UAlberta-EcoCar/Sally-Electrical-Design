################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/EcoLib/drivers/bme280/bme280.c 

OBJS += \
./lib/EcoLib/drivers/bme280/bme280.o 

C_DEPS += \
./lib/EcoLib/drivers/bme280/bme280.d 


# Each subdirectory must supply rules for building sources it contributes
lib/EcoLib/drivers/bme280/%.o lib/EcoLib/drivers/bme280/%.su lib/EcoLib/drivers/bme280/%.cyclo: ../lib/EcoLib/drivers/bme280/%.c lib/EcoLib/drivers/bme280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/ssd1306/inc" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/bme280" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util/log" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/ecocan" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/MQ8" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/EEPROM-Emulator/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-EcoLib-2f-drivers-2f-bme280

clean-lib-2f-EcoLib-2f-drivers-2f-bme280:
	-$(RM) ./lib/EcoLib/drivers/bme280/bme280.cyclo ./lib/EcoLib/drivers/bme280/bme280.d ./lib/EcoLib/drivers/bme280/bme280.o ./lib/EcoLib/drivers/bme280/bme280.su

.PHONY: clean-lib-2f-EcoLib-2f-drivers-2f-bme280

