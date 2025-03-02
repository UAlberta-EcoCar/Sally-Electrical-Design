################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD/src/font12.c \
../LCD/src/font16.c \
../LCD/src/font20.c \
../LCD/src/font24.c \
../LCD/src/font8.c \
../LCD/src/ili9488.c \
../LCD/src/lcd_io_spi.c \
../LCD/src/stm32_adafruit_lcd.c 

OBJS += \
./LCD/src/font12.o \
./LCD/src/font16.o \
./LCD/src/font20.o \
./LCD/src/font24.o \
./LCD/src/font8.o \
./LCD/src/ili9488.o \
./LCD/src/lcd_io_spi.o \
./LCD/src/stm32_adafruit_lcd.o 

C_DEPS += \
./LCD/src/font12.d \
./LCD/src/font16.d \
./LCD/src/font20.d \
./LCD/src/font24.d \
./LCD/src/font8.d \
./LCD/src/ili9488.d \
./LCD/src/lcd_io_spi.d \
./LCD/src/stm32_adafruit_lcd.d 


# Each subdirectory must supply rules for building sources it contributes
LCD/src/%.o LCD/src/%.su LCD/src/%.cyclo: ../LCD/src/%.c LCD/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/dash9/OneDrive/Documents/School/EcoCar Club/v2/Sally-Electrical-Design/LCD Test/LCD/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LCD-2f-src

clean-LCD-2f-src:
	-$(RM) ./LCD/src/font12.cyclo ./LCD/src/font12.d ./LCD/src/font12.o ./LCD/src/font12.su ./LCD/src/font16.cyclo ./LCD/src/font16.d ./LCD/src/font16.o ./LCD/src/font16.su ./LCD/src/font20.cyclo ./LCD/src/font20.d ./LCD/src/font20.o ./LCD/src/font20.su ./LCD/src/font24.cyclo ./LCD/src/font24.d ./LCD/src/font24.o ./LCD/src/font24.su ./LCD/src/font8.cyclo ./LCD/src/font8.d ./LCD/src/font8.o ./LCD/src/font8.su ./LCD/src/ili9488.cyclo ./LCD/src/ili9488.d ./LCD/src/ili9488.o ./LCD/src/ili9488.su ./LCD/src/lcd_io_spi.cyclo ./LCD/src/lcd_io_spi.d ./LCD/src/lcd_io_spi.o ./LCD/src/lcd_io_spi.su ./LCD/src/stm32_adafruit_lcd.cyclo ./LCD/src/stm32_adafruit_lcd.d ./LCD/src/stm32_adafruit_lcd.o ./LCD/src/stm32_adafruit_lcd.su

.PHONY: clean-LCD-2f-src

