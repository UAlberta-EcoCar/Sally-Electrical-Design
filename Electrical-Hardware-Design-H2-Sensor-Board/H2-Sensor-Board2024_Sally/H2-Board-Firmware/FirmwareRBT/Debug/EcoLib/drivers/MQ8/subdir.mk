################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/drivers/MQ8/mq8calibration.c 

OBJS += \
./EcoLib/drivers/MQ8/mq8calibration.o 

C_DEPS += \
./EcoLib/drivers/MQ8/mq8calibration.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/drivers/MQ8/%.o EcoLib/drivers/MQ8/%.su EcoLib/drivers/MQ8/%.cyclo: ../EcoLib/drivers/MQ8/%.c EcoLib/drivers/MQ8/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-drivers-2f-MQ8

clean-EcoLib-2f-drivers-2f-MQ8:
	-$(RM) ./EcoLib/drivers/MQ8/mq8calibration.cyclo ./EcoLib/drivers/MQ8/mq8calibration.d ./EcoLib/drivers/MQ8/mq8calibration.o ./EcoLib/drivers/MQ8/mq8calibration.su

.PHONY: clean-EcoLib-2f-drivers-2f-MQ8

