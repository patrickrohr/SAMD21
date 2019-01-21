# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

project(samd21_freertos)

################################################################
# Board Support Package
################################################################
set(Kconfiglib_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config")
set(Kconfiglib_DOT_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config")
set(Kconfiglib_CONFIG_FILENAME "Config.in")
set(Kconfiglib_HEADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}")
find_package(Kconfiglib QUIET)

set(FreeRTOS_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config/FreeRTOS")
find_package(FreeRTOS REQUIRED)

add_library(samd21_freertos STATIC)

target_sources(samd21_freertos
    PRIVATE
        # Peripheral Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/peripherals/src/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/peripherals/src/gclk.c
)

target_include_directories(samd21_freertos
    PRIVATE
        # Peripheral Includes
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/peripherals/src
)

target_include_directories(samd21_freertos
    PUBLIC
        # Peripheral Includes, should be private. TODO.
        ${CMAKE_CURRENT_SOURCE_DIR}/peripherals/include
)

target_link_libraries(samd21_freertos
    PUBLIC
        FreeRTOS
    PRIVATE
        cmsis_samd21
        m
)

target_compile_features(samd21_freertos PRIVATE c_static_assert)

# Generate configuration before before building
if(Kconfiglib_FOUND)
    add_dependencies(samd21_freertos genconfig)
endif()


################################################################
# Example App
################################################################
add_executable(app)

target_sources(app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/example-app/src/main.c
)

target_link_libraries(app
    PRIVATE
        cmsis_samd21 # TODO: Remove
        samd21_freertos # Links in FreeRTOS as well
)

# TODO: Clean up
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${CMAKE_CURRENT_BINARY_DIR}/app ${CMAKE_BINARY_DIR}/app.bin
)
