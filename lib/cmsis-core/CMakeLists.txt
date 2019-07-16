# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(cmsis_core)

include(FetchContent)

FetchContent_Declare(cmsis_core_download
    GIT_REPOSITORY https://github.com/ARMmbed/cmsis-core.git
    GIT_TAG        v1.2.0
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(cmsis_core_download)

if (NOT cmsis_core_download_POPULATED)
    FetchContent_Populate(cmsis_core_download)

    add_library(cmsis_core INTERFACE)

    target_include_directories(cmsis_core INTERFACE
        ${cmsis_core_download_SOURCE_DIR}/cmsis-core
    )
endif()
