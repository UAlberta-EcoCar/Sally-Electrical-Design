################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.c \
../EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.c 

OBJS += \
./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.o \
./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.o 

C_DEPS += \
./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.d \
./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/drivers/EEPROM-Emulator/Src/%.o EcoLib/drivers/EEPROM-Emulator/Src/%.su EcoLib/drivers/EEPROM-Emulator/Src/%.cyclo: ../EcoLib/drivers/EEPROM-Emulator/Src/%.c EcoLib/drivers/EEPROM-Emulator/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/ecocan" -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/util/typedef" -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/util/log" -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/drivers/semtech/hoperf/rfm95/inc" -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/drivers/semtech/sx1281/inc" -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/drivers/sdcard/inc" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/abina/Desktop/side/EcoCar/RF-Module-UltraCom-Series-1/RF-Board-Sht/EcoLib/drivers/EEPROM-Emulator/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-drivers-2f-EEPROM-2d-Emulator-2f-Src

clean-EcoLib-2f-drivers-2f-EEPROM-2d-Emulator-2f-Src:
	-$(RM) ./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.cyclo ./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.d ./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.o ./EcoLib/drivers/EEPROM-Emulator/Src/eeprom_emul.su ./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.cyclo ./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.d ./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.o ./EcoLib/drivers/EEPROM-Emulator/Src/flash_interface.su

.PHONY: clean-EcoLib-2f-drivers-2f-EEPROM-2d-Emulator-2f-Src

