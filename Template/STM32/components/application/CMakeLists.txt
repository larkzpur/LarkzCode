add_library(application
    src/blink_use_case.cpp
)

target_include_directories(application PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(application PUBLIC
    abstraction
    board_config
)

target_compile_options(application PRIVATE
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
