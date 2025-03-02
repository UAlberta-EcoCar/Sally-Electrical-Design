################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.c 

OBJS += \
./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.o 

C_DEPS += \
./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/drivers/semtech/hoperf/rfm95/src/%.o EcoLib/drivers/semtech/hoperf/rfm95/src/%.su EcoLib/drivers/semtech/hoperf/rfm95/src/%.cyclo: ../EcoLib/drivers/semtech/hoperf/rfm95/src/%.c EcoLib/drivers/semtech/hoperf/rfm95/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../EcoLib/ecocan -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src

clean-EcoLib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src:
	-$(RM) ./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.cyclo ./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.d ./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.o ./EcoLib/drivers/semtech/hoperf/rfm95/src/rf-rfm95.su

.PHONY: clean-EcoLib-2f-drivers-2f-semtech-2f-hoperf-2f-rfm95-2f-src

