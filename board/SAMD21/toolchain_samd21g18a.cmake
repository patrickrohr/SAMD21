# Major TODO: Clean all of this up, set up configurations

# CMake options to cross-compile for Atmel SAMD21G18A from Linux on PC
set(TOOLCHAIN "Target")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER /usr/bin/arm-none-eabi-as)
set(CMAKE_OBJCOPY /usr/bin/arm-none-eabi-objcopy)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

add_compile_options(-mtune=cortex-m0plus)
add_compile_options(-mthumb)
add_compile_options(-march=armv6-m)
add_compile_options($<$<CONFIG:Debug>:-ggdb>)
add_compile_options($<$<CONFIG:Debug>:-gdwarf-2>)
add_compile_options($<$<CONFIG:Debug>:-g3>)
