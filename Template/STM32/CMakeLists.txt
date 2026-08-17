cmake_minimum_required(VERSION 3.20)

if(NOT DEFINE CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE 
        ${CMAKE_CURRENT_LIST_DIR}/cmake/arm-none-eabi-gcc.cmake
        CACHE FILEPATH "CMake toolchain file"
    )
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(nucleo_f411_blink cxx ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debugt CACHE STRING "Build type" FORCE)
endif()

set(MCPU cortex-m4)
set(MFLOAT_ABI soft)
set(MFPU fpv4-sp-d16)
# This is the default for a fresh configure only.
# For an existing build folderm, override it with -DNUCLEO_SYSCLICK