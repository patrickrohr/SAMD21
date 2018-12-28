# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(cmsis_samd21)

include(FetchContent)

FetchContent_Declare(
    cmsis_samd21_download
    GIT_REPOSITORY https://github.com/ARMmbed/cmsis-core-atmel-samcortexm0p-samd21
    GIT_TAG master
)

FetchContent_GetProperties(cmsis_samd21_download)

if (NOT cmsis_samd21_download_POPULATED)
    FetchContent_Populate(cmsis_samd21_download)

    add_library(cmsis_samd21 INTERFACE)

    target_include_directories(cmsis_samd21 INTERFACE
        ${cmsis_samd21_download_SOURCE_DIR}/source
        ${cmsis_samd21_download_SOURCE_DIR}/source/TARGET_SAMD21G18A/TOOLCHAIN_GCC_ARM
        ${cmsis_samd21_download_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/source
        ${cmsis_samd21_download_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include
        ${cmsis_samd21_download_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/component
        ${cmsis_samd21_download_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/instance
        ${cmsis_samd21_download_SOURCE_DIR}/source/cmsis/TARGET_SAMD21/include/pio
        ${cmsis_samd21_download_SOURCE_DIR}/source/header_files
        ${cmsis_samd21_download_SOURCE_DIR}/source/preprocessor
    )

    target_link_libraries(cmsis_samd21 INTERFACE cmsis_core)
endif()
