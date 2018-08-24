# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(samd21)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main.c
    )

add_executable(samd21 ${SOURCES})

target_link_libraries(samd21 PRIVATE cmsis_samd21)