# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Compiler options
target_compile_options(${BUILD_UNIT_0_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>: ${CUBE_CMAKE_C_FLAGS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${CUBE_CMAKE_CXX_FLAGS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${CUBE_CMAKE_ASM_FLAGS}>
)

# Linker options
target_link_options(${BUILD_UNIT_0_NAME} PRIVATE ${CUBE_CMAKE_EXE_LINKER_FLAGS})

# Add sources to executable/library
target_sources(${BUILD_UNIT_0_NAME} PRIVATE
    "Core/Src/app_freertos.c"
    "Core/Src/dma.c"
    "Core/Src/fdcan.c"
    "Core/Src/gpio.c"
    "Core/Src/i2c.c"
    "Core/Src/main.c"
    "Core/Src/spi.c"
    "Core/Src/stm32g4xx_hal_msp.c"
    "Core/Src/stm32g4xx_hal_timebase_tim.c"
    "Core/Src/stm32g4xx_it.c"
    "Core/Src/syscalls.c"
    "Core/Src/sysmem.c"
    "Core/Src/system_stm32g4xx.c"
    "Core/Src/usart.c"
    "Core/Startup/startup_stm32g484retx.s"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c"
    "Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c"
    "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
    "Middlewares/Third_Party/FreeRTOS/Source/croutine.c"
    "Middlewares/Third_Party/FreeRTOS/Source/event_groups.c"
    "Middlewares/Third_Party/FreeRTOS/Source/list.c"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
    "Middlewares/Third_Party/FreeRTOS/Source/queue.c"
    "Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c"
    "Middlewares/Third_Party/FreeRTOS/Source/tasks.c"
    "Middlewares/Third_Party/FreeRTOS/Source/timers.c"
)

target_include_directories(${BUILD_UNIT_0_NAME} PRIVATE
    "Core/Inc"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/include&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Device/ST/STM32G4xx/Include&quot;"
    "/Users/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Include&quot;"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Drivers/STM32G4xx_HAL_Driver/Inc"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Drivers/CMSIS/Device/ST/STM32G4xx/Include"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Drivers/CMSIS/Include"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Middlewares/Third_Party/FreeRTOS/Source/include"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.5.2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/include"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Device/ST/STM32G4xx/Include"
    "Users/gfran/STM32Cube/Repository/STM32Cube_FW_G4_V1.6.1/Drivers/CMSIS/Include"
    "."
    "Drivers/STM32G4xx_HAL_Driver/Inc"
    "Drivers/STM32G4xx_HAL_Driver/Inc/Legacy"
    "Middlewares/Third_Party/FreeRTOS/Source/include"
    "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "Drivers/CMSIS/Device/ST/STM32G4xx/Include"
    "Drivers/CMSIS/Include"
)

configure_file("${CMAKE_SOURCE_DIR}/STM32G484RETX_FLASH.ld" "${CMAKE_BINARY_DIR}" COPYONLY)

set_target_properties(${BUILD_UNIT_0_NAME} PROPERTIES LINK_DEPENDS "STM32G484RETX_FLASH.ld")

