################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/drivers/bme280/bme280.c 

OBJS += \
./EcoLib/drivers/bme280/bme280.o 

C_DEPS += \
./EcoLib/drivers/bme280/bme280.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/drivers/bme280/%.o EcoLib/drivers/bme280/%.su EcoLib/drivers/bme280/%.cyclo: ../EcoLib/drivers/bme280/%.c EcoLib/drivers/bme280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-drivers-2f-bme280

clean-EcoLib-2f-drivers-2f-bme280:
	-$(RM) ./EcoLib/drivers/bme280/bme280.cyclo ./EcoLib/drivers/bme280/bme280.d ./EcoLib/drivers/bme280/bme280.o ./EcoLib/drivers/bme280/bme280.su

.PHONY: clean-EcoLib-2f-drivers-2f-bme280

