# ARM Cortex-M Toolchain for STM32
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE FILEPATH "ASM compiler")
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy" CACHE FILEPATH "Object copy")
set(CMAKE_OBJDUMP "arm-none-eabi-objdump" CACHE FILEPATH "Object dump")
set(CMAKE_AR "arm-none-eabi-ar" CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB "arm-none-eabi-ranlib" CACHE FILEPATH "Ranlib")

# Skip compiler tests (they fail for embedded systems)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Don't run linker tests
set(CMAKE_C_ABI_COMPILED 1)
set(CMAKE_CXX_ABI_COMPILED 1)

# Program extensions for embedded targets
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
