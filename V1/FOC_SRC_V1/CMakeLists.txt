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


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME FOC_SRC_V1)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Include external Libraries
# FetchContent Documentation: https://cmake.org/cmake/help/latest/module/FetchContent.html
include(FetchContent)

# Import repository into the Modules folder
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/Modules)

FetchContent_Declare(
    # FetchContent Name is case-insensitive
    ecolib
    GIT_REPOSITORY git@github.com:UAlberta-EcoCar/EcoLib.git
    GIT_TAG v0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    SYSTEM
)
# more `FetchContent_Declare`s (if any). They should all be declared
# before any calls to FetchContent_Make_available (see docs for why).

FetchContent_MakeAvailable(EcoLib)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/Src/ws2812.c
    Modules/ecolib-src/ecocan/ecocar_can.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Modules/ecolib-src/ecocan 
    Modules/ecolib-src/util/typedef
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
