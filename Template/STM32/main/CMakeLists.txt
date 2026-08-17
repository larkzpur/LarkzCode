add_library(app_main
    main.cpp
)

target_include_directories(app_main PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../components/platform/include
)

target_link_libraries(app_main PUBLIC
    platform
    infrastructure_gpio
    application
)


target_compile_options(app_main PRIVATE
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