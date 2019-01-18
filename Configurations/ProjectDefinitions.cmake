
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)

if(TOOLCHAIN STREQUAL "Target")
    # For future reference CMake 3.13 is adding target_link_options
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/Configurations/SAMD21/samd21g18a_flash.ld")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m0 -mthumb")
endif()

add_compile_options(-O0)
add_compile_options(-D__ATSAMD21G18A__)
add_compile_options(-Wall)
