################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/EcoLib/drivers/ssd1306/src/ssd1306.c \
../lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.c \
../lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.c 

OBJS += \
./lib/EcoLib/drivers/ssd1306/src/ssd1306.o \
./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.o \
./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.o 

C_DEPS += \
./lib/EcoLib/drivers/ssd1306/src/ssd1306.d \
./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.d \
./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
lib/EcoLib/drivers/ssd1306/src/%.o lib/EcoLib/drivers/ssd1306/src/%.su lib/EcoLib/drivers/ssd1306/src/%.cyclo: ../lib/EcoLib/drivers/ssd1306/src/%.c lib/EcoLib/drivers/ssd1306/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/ssd1306/inc" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/bme280" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util/log" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/util" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/ecocan" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/MQ8" -I"C:/Users/abina/Desktop/side/EcoCar/H2BoardSally/Electrical-Hardware-Design-H2-Sensor-Board/H2-Sensor-Board2024_Sally/H2-Board-Firmware/FirmwareRBT/lib/EcoLib/drivers/EEPROM-Emulator/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-EcoLib-2f-drivers-2f-ssd1306-2f-src

clean-lib-2f-EcoLib-2f-drivers-2f-ssd1306-2f-src:
	-$(RM) ./lib/EcoLib/drivers/ssd1306/src/ssd1306.cyclo ./lib/EcoLib/drivers/ssd1306/src/ssd1306.d ./lib/EcoLib/drivers/ssd1306/src/ssd1306.o ./lib/EcoLib/drivers/ssd1306/src/ssd1306.su ./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.cyclo ./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.d ./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.o ./lib/EcoLib/drivers/ssd1306/src/ssd1306_fonts.su ./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.cyclo ./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.d ./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.o ./lib/EcoLib/drivers/ssd1306/src/ssd1306_tests.su

.PHONY: clean-lib-2f-EcoLib-2f-drivers-2f-ssd1306-2f-src

