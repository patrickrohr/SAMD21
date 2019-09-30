# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(hal_test_app)

add_executable(hal_test_app)

target_sources(hal_test_app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(hal_test_app
    PRIVATE
        samd21_hal
)

samd_target_install_binary(hal_test_app ${CMAKE_BINARY_DIR})
