# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(cmsis_samd21)

include(ExternalProject)

ExternalProject_Add(
    cmsis_samd21_Download
    GIT_REPOSITORY https://github.com/ARMmbed/cmsis-core-atmel-samcortexm0p-samd21
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    LOG_DOWNLOAD ON
    )

ExternalProject_Get_Property(cmsis_samd21_Download SOURCE_DIR)
set(cmsis_samd21_SOURCE_DIR ${SOURCE_DIR})

add_library(cmsis_samd21 INTERFACE)

add_dependencies(cmsis_samd21 cmsis_samd21_Download)

target_include_directories(cmsis_samd21 INTERFACE
    ${cmsis_samd21_SOURCE_DIR}/source
    ${cmsis_samd21_SOURCE_DIR}/source/TARGET_SAMD21G18A/TOOLCHAIN_GCC_ARM
    ${cmsis_samd21_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/source
    ${cmsis_samd21_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include
    ${cmsis_samd21_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/component
    ${cmsis_samd21_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/instance
    ${cmsis_samd21_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/pio
    ${cmsis_samd21_SOURCE_DIR}/source/header_files
    ${cmsis_samd21_SOURCE_DIR}/source/preprocessor
    )


target_link_libraries(cmsis_samd21 INTERFACE cmsis_core)