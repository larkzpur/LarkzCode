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
# For an existing build folderm, override it with -DNUCLEO_SYSCLICK_84MHZ=ON/OFF
# You can also select 42MHz with -DNUCLEO_SYSCLICK_42MHZ=ON.

#Use 84MHz
#cmake -S . -B build -G Ninja -DNUCLEO_SYSCLICK_84MHZ=ON
#cmake --build build

#Use 42MHz
#cmake -S . -B build -G Ninja -DNUCLEO_SYSCLICK_42MHZ=ON
#cmake --build build

#Use 16MHz
#cmake -S . -B build -G Ninja -DNUCLEO_SYSCLICK_84MHZ=OFF
#cmake --build build
#verify active value
#findstr /I NUCLEO_SYSCLK_84MHZ CMakeCache.txt

option(NUCLEO_SYSCLK_84MHZ "Run core clock at 84MHz from PLL (HSI source)" OFF)
option(NUCLEO_SYSCLK_42MHZ "Run core clock at 42MHz from PLL (HSI source)" OFF)

#Add component libraries
add-subdirectory(components/abstraction)
add-subdirectory(components/board_config)
add-subdirectory(components/infrastructure_gpio)
add-subdirectory(components/application)
add-subdirectory(main)

add_executable(${PROJECT_NAME}.elf
    startup/startup-stm32f411xe.s
)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    app_main
)

if(NUCLEO_SYSCLK_84MHZ)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE USE_SYSCLK_84MHZ=1)
elseif(NUCLEO_SYSCLK_42MHZ)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE USE_SYSCLK_42MHZ=1)
endif()

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=${MCPU}
    -mthumb
    -mfpi=${MFPU}
    -mfloat-abi=${MFLOAT_ABI}
    -fno-exceptions
    -ffunction-sections
    -fdat-sections
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
    -T${CMAKE_SOURCE_DIR}/STM32F411RETx_ld
    -mcpu=${MCPU}
    -mthumb
    -mfpi=${MFPU}
    -mfloat-abi=${MFLOAT_ABI}
    -Wl, --gc-sections
    -Wl, -Map=${PROJECT_NAME}.map
    -nostartfiles
    -nostdlib
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
)