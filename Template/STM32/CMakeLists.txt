cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE 
        ${CMAKE_CURRENT_LIST_DIR}/cmake/arm-none-eabi-gcc.cmake
        CACHE FILEPATH "CMake toolchain file"
    )
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(nucleo_f411_blink CXX ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(MCPU cortex-m4)
set(MFLOAT_ABI soft)
set(MFPU fpv4-sp-d16)

# Core clock frequency: 84MHz from PLL (HSI source)
set(NUCLEO_SYSCLK_FREQ 84)

#Add component libraries
add_subdirectory(components/abstraction)
add_subdirectory(components/board_config)
add_subdirectory(components/platform)
add_subdirectory(components/infrastructure_gpio)
add_subdirectory(components/application)
add_subdirectory(main)

add_executable(${PROJECT_NAME}.elf
    startup/startup_stm32f411xe.s
)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    app_main
)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE USE_SYSCLK_${NUCLEO_SYSCLK_FREQ}MHZ=1)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=${MCPU}
    -mthumb
    -mfpu=${MFPU}
    -mfloat-abi=${MFLOAT_ABI}
    -fno-exceptions
    -fno-rtti
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -Wpedantic
    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:RelWithDebInfo>:-Og>
    $<$<CONFIG:RelWithDebInfo>:-g3>
    $<$<CONFIG:Release>: -O2>
    $<$<CONFIG:MinSizeRel>:-Os>
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F411RETx_FLASH.ld
    -mcpu=${MCPU}
    -mthumb
    -mfpu=${MFPU}
    -mfloat-abi=${MFLOAT_ABI}
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -nostartfiles
    -nostdlib
    -lsupc++
    -lgcc
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-size $<TARGET_FILE:${PROJECT_NAME}.elf>
)