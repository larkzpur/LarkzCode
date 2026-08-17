add_library(abstraction 
    src/blink_use_case.cpp
)

target_include_directories(abstraction PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(abstraction PUBLIC
    abstraction
    board_config
)

target_compile_options(
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

