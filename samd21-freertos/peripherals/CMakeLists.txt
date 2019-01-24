# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(samd21_peripherals)

add_library(samd21_peripherals STATIC)

target_sources(samd21_peripherals
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gclk.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sercom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/uart.c
)

target_include_directories(samd21_peripherals
    PRIVATE
        ${Kconfiglib_HEADER_OUTPUT}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(samd21_peripherals
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(samd21_peripherals
    PRIVATE
        cmsis_samd21
        m
)

target_compile_features(samd21_peripherals PRIVATE c_static_assert)

# Generate configuration before before building
if(Kconfiglib_FOUND)
    add_dependencies(samd21_peripherals genconfig)
endif()
