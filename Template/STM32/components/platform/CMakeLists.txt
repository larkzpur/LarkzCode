add_library(platform 
    src/system_stm32f4xx.cpp
    src/cxx_runtime.cpp
)

target_include_directories(platform PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_options(platform PUBLIC
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=soft
    -fno-exceptions
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -Wpedantic
    -O2
)