
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)

if(TOOLCHAIN STREQUAL "Target")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/Configurations/SAMD21/samd21g18a_flash.ld")
endif()

add_compile_options(-O0)
add_compile_options(-D__ATSAMD21G18A__)
