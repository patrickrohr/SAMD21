# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(samd21_hal_utils)

add_library(samd21_hal_utils)

target_sources(samd21_hal_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/irq.cpp
)

target_include_directories(samd21_hal_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(samd21_hal_utils
    PUBLIC
        common_utils)

target_link_libraries(samd21_hal_utils
    PRIVATE
        cmsis_samd21
)

target_compile_features(samd21_hal_utils
    PUBLIC
        cxx_constexpr
)

add_test_directory(tests)
