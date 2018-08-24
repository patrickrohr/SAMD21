# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(app)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main.c
    )

add_executable(app ${SOURCES})

target_link_libraries(app PRIVATE cmsis_samd21)