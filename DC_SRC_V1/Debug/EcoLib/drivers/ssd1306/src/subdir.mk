################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EcoLib/drivers/ssd1306/src/ssd1306.c \
../EcoLib/drivers/ssd1306/src/ssd1306_fonts.c \
../EcoLib/drivers/ssd1306/src/ssd1306_tests.c 

OBJS += \
./EcoLib/drivers/ssd1306/src/ssd1306.o \
./EcoLib/drivers/ssd1306/src/ssd1306_fonts.o \
./EcoLib/drivers/ssd1306/src/ssd1306_tests.o 

C_DEPS += \
./EcoLib/drivers/ssd1306/src/ssd1306.d \
./EcoLib/drivers/ssd1306/src/ssd1306_fonts.d \
./EcoLib/drivers/ssd1306/src/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
EcoLib/drivers/ssd1306/src/%.o EcoLib/drivers/ssd1306/src/%.su EcoLib/drivers/ssd1306/src/%.cyclo: ../EcoLib/drivers/ssd1306/src/%.c EcoLib/drivers/ssd1306/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../EcoLib/ecocan -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-EcoLib-2f-drivers-2f-ssd1306-2f-src

clean-EcoLib-2f-drivers-2f-ssd1306-2f-src:
	-$(RM) ./EcoLib/drivers/ssd1306/src/ssd1306.cyclo ./EcoLib/drivers/ssd1306/src/ssd1306.d ./EcoLib/drivers/ssd1306/src/ssd1306.o ./EcoLib/drivers/ssd1306/src/ssd1306.su ./EcoLib/drivers/ssd1306/src/ssd1306_fonts.cyclo ./EcoLib/drivers/ssd1306/src/ssd1306_fonts.d ./EcoLib/drivers/ssd1306/src/ssd1306_fonts.o ./EcoLib/drivers/ssd1306/src/ssd1306_fonts.su ./EcoLib/drivers/ssd1306/src/ssd1306_tests.cyclo ./EcoLib/drivers/ssd1306/src/ssd1306_tests.d ./EcoLib/drivers/ssd1306/src/ssd1306_tests.o ./EcoLib/drivers/ssd1306/src/ssd1306_tests.su

.PHONY: clean-EcoLib-2f-drivers-2f-ssd1306-2f-src

