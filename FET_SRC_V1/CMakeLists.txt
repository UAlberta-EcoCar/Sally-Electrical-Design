cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# tinyUSB
add_compile_definitions(CFG_TUSB_MCU=OPT_MCU_STM32G4)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME FET_SRC_V1)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  # Add user sources here
  tinyUSB/src/tusb.c
  tinyUSB/src/class/cdc/cdc_device.c
  tinyUSB/src/class/cdc/cdc_host.c
  tinyUSB/src/class/cdc/cdc_rndis_host.c
  tinyUSB/src/common/tusb_fifo.c
  tinyUSB/src/device/usbd.c
  tinyUSB/src/device/usbd_control.c
  tinyUSB/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c

  Core/Src/usb_descriptors.c
  EcoLib/ecocan/ecocar_can.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  # Add user defined include paths
  tinyUSB/hw
  tinyUSB/src
  EcoLib/ecocan
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
