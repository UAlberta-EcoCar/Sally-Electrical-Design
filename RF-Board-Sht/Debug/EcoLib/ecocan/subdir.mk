################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/ecocan/ecocar_can.c 

OBJS += \
./EcoLib/ecocan/ecocar_can.o 

C_DEPS += \
./EcoLib/ecocan/ecocar_can.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/ecocan/%.o EcoLib/ecocan/%.su EcoLib/ecocan/%.cyclo: ../EcoLib/ecocan/%.c EcoLib/ecocan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/RF-Board-Sht/EcoLib/ecocan" -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/RF-Board-Sht/EcoLib/util/typedef" -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/RF-Board-Sht/EcoLib/util/log" -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/RF-Board-Sht/EcoLib/drivers/semtech/hoperf/rfm95/inc" -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/RF-Board-Sht/EcoLib/drivers/semtech/sx1281/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-ecocan

clean-EcoLib-2f-ecocan:
	-$(RM) ./EcoLib/ecocan/ecocar_can.cyclo ./EcoLib/ecocan/ecocar_can.d ./EcoLib/ecocan/ecocar_can.o ./EcoLib/ecocan/ecocar_can.su

.PHONY: clean-EcoLib-2f-ecocan

