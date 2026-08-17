add_library(infrastructure_gpio 
    src/gpio_led.cpp
)

target_include_directories(infrastructure_gpio PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(infrastructure_gpio PUBLIC
    abstraction
    board_config
)

target_compile_options(infrastructure_gpio PRIVATE
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