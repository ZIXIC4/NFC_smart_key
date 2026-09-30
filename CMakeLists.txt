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
set(CMAKE_PROJECT_NAME NFC_smart_key)

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

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Drivers/STM32G0xx_HAL_Driver/Inc
)

# PN5180 Type A / ISO and MIFARE Classic Reader Library sources.
set(PN5180_PORT_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/PN5180_TypeA_ISO_Port
)

file(STRINGS
    ${PN5180_PORT_ROOT}/sources_core.txt
    PN5180_CORE_SOURCES
)
list(TRANSFORM PN5180_CORE_SOURCES PREPEND "${PN5180_PORT_ROOT}/")

file(STRINGS
    ${PN5180_PORT_ROOT}/include_dirs.txt
    PN5180_INCLUDE_DIRS
)
list(TRANSFORM PN5180_INCLUDE_DIRS PREPEND "${PN5180_PORT_ROOT}/")

set(PN5180_STM32_PORT_SOURCES
    ${PN5180_PORT_ROOT}/Platform/DAL/src/phbalReg.c
    ${PN5180_PORT_ROOT}/Platform/DAL/src/phDriver_Gpio.c
    ${PN5180_PORT_ROOT}/Platform/DAL/src/phDriver_PinMap.c
    ${PN5180_PORT_ROOT}/Platform/DAL/src/phDriver_Timer.c
    ${PN5180_PORT_ROOT}/RTOS/phOsal/src/NullOs/phOsal_NullOs.c
    ${PN5180_PORT_ROOT}/RTOS/phOsal/src/NullOs/portable/phOsal_Port_CM3.c
)

add_library(pn5180_typea_iso OBJECT
    ${PN5180_CORE_SOURCES}
    ${PN5180_STM32_PORT_SOURCES}
)

target_link_libraries(pn5180_typea_iso PRIVATE
    stm32cubemx
)

target_include_directories(pn5180_typea_iso PRIVATE
    ${PN5180_INCLUDE_DIRS}
)

target_compile_definitions(pn5180_typea_iso PRIVATE
    NXPBUILD_CUSTOMER_HEADER_INCLUDED
    PH_OSAL_NULLOS
    PHDRIVER_STM32_PN5180_BOARD
)

# Application headers and the Reader Library objects share the same component
# selection and include paths.
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${PN5180_INCLUDE_DIRS}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    NXPBUILD_CUSTOMER_HEADER_INCLUDED
    PH_OSAL_NULLOS
    PHDRIVER_STM32_PN5180_BOARD
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    pn5180_typea_iso

    # Add user defined libraries
)
