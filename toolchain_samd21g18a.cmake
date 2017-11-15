#CMake options to cross-compile for Atmel SAMD21G18A from Linux on PC
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER /usr/bin/arm-none-eabi-as)