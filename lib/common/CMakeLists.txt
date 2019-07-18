# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(common_utils)

add_library(common_utils)

target_sources(common_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/irq.cpp
)

target_include_directories(common_utils
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common_utils
    PRIVATE
        cmsis_samd21
)

target_compile_features(common_utils
    PUBLIC
        cxx_constexpr
)

configure_file(cmakeconfig.h.in cmakeconfig.h)
add_test_directory(test)
