# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(common_utils)

add_library(common_utils INTERFACE)

# target_sources(common_utils
#     PUBLIC
#         # ${CMAKE_CURRENT_SOURCE_DIR}/include/hardware_at.hpp
#     )

target_include_directories(common_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_compile_features(common_utils INTERFACE cxx_constexpr)
# target_compile_features(common_utils PRIVATE)

add_unit_test_directory(test)
