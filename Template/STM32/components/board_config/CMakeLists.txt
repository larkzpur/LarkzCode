add_library(board_config INTERFACE)

target_include_directories(board_config INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)