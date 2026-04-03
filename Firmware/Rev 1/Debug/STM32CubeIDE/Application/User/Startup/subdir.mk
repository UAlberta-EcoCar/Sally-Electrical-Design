################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../STM32CubeIDE/Application/User/Startup/startup_stm32g491metx.s 

OBJS += \
./STM32CubeIDE/Application/User/Startup/startup_stm32g491metx.o 

S_DEPS += \
./STM32CubeIDE/Application/User/Startup/startup_stm32g491metx.d 


# Each subdirectory must supply rules for building sources it contributes
STM32CubeIDE/Application/User/Startup/%.o: ../STM32CubeIDE/Application/User/Startup/%.s STM32CubeIDE/Application/User/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g -DDEBUG -c -I../../Core/Inc -I../../USB_Device/App -I../../USB_Device/Target -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-STM32CubeIDE-2f-Application-2f-User-2f-Startup

clean-STM32CubeIDE-2f-Application-2f-User-2f-Startup:
	-$(RM) ./STM32CubeIDE/Application/User/Startup/startup_stm32g491metx.d ./STM32CubeIDE/Application/User/Startup/startup_stm32g491metx.o

.PHONY: clean-STM32CubeIDE-2f-Application-2f-User-2f-Startup

