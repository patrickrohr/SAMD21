# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(app)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    )

add_executable(app ${SOURCES})

target_link_libraries(app PRIVATE cmsis_samd21 samd21_peripherals)

# TODO: Clean up
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${CMAKE_CURRENT_BINARY_DIR}/app ${CMAKE_BINARY_DIR}/app.bin
)
