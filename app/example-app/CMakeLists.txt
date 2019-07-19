# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(app)

add_executable(app)

target_sources(app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_link_libraries(app
    PRIVATE
        cmsis_samd21 # TODO: Remove
        samd21_hal
)

# TODO: Clean up
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${CMAKE_CURRENT_BINARY_DIR}/app ${CMAKE_BINARY_DIR}/app.bin
)
