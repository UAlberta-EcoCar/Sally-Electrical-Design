################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.c 

OBJS += \
./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.o 

C_DEPS += \
./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.d 


# Each subdirectory must supply rules for building sources it contributes
ecolib/drivers/semtech/hoperf/rfm95/src/%.o ecolib/drivers/semtech/hoperf/rfm95/src/%.su ecolib/drivers/semtech/hoperf/rfm95/src/%.cyclo: ../ecolib/drivers/semtech/hoperf/rfm95/src/%.c ecolib/drivers/semtech/hoperf/rfm95/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G484xx -c -I../Core/Inc -I"C:/Users/abina/Desktop/EcoCar/RF-Board-UltraComm-Series-1/Firmware/ecolib/drivers/semtech/hoperf/rfm95/inc" -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Device/ST/STM32G4xx/Include -IC:/Users/abina/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ecolib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src

clean-ecolib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src:
	-$(RM) ./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.cyclo ./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.d ./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.o ./ecolib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.su

.PHONY: clean-ecolib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src

