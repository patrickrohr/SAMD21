# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(samd21_hal)

add_library(samd21_hal STATIC)

target_sources(samd21_hal
    PRIVATE
        # TODO: Remove c files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gclk.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sercom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usart.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock/clock_source_generic.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock/dfll48m.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock/osc8m.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clock/xosc32k.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port/pin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/serial/sercom_base.cpp
)

target_compile_features(samd21_hal
    PUBLIC
        cxx_constexpr
    )

target_include_directories(samd21_hal
    PRIVATE
        ${Kconfiglib_HEADER_OUTPUT}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(samd21_hal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(samd21_hal
    PRIVATE
        cmsis_samd21
        m
)

target_link_libraries(samd21_hal
    PUBLIC
        common_utils
        samd21_hal_utils
)

target_compile_features(samd21_hal PRIVATE c_static_assert)

# Generate configuration before before building
if(Kconfiglib_FOUND)
    add_dependencies(samd21_hal genconfig)
endif()
