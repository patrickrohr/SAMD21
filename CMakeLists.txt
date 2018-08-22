# Copyrights: Patrick Rohr, 2017

cmake_minimum_required(VERSION 3.9.1)

project(samd21 C)



# This stuff should probably move to a config file...
add_definitions(-D__ATSAMD21G18A__)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)

set(SOURCES
    src/main.c    
    )

set(INCLUDE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    )

set(EXE samd21)

add_executable(${EXE} ${SOURCES})
include_directories(${INCLUDE_DIR})

target_compile_options(${EXE} PUBLIC -O0 -mtune=cortex-m0 -mthumb -march=armv6-m)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cmsis-core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cmsis-core-samcortexm0p-samd21)

# target_link_libraries(samd21 INTERFACE cmsis_core)

# add_dependencies(samd21 cmsis_core_download)

# TODO: use find library
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/BufferPool)
#target_link_libraries(samd21 PUBLIC BufferPool)
