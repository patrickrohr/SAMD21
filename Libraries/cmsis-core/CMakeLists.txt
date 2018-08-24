# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(cmsis_core)

include(ExternalProject)

ExternalProject_Add(
    cmsis_core_Download
    GIT_REPOSITORY https://github.com/ARMmbed/cmsis-core.git
    GIT_TAG v1.2.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    LOG_DOWNLOAD ON
    )

ExternalProject_Get_Property(cmsis_core_Download SOURCE_DIR)
set(cmsis_core_SOURCE_DIR ${SOURCE_DIR})

add_library(cmsis_core INTERFACE)

add_dependencies(cmsis_core cmsis_core_Download)

target_include_directories(cmsis_core INTERFACE
    ${cmsis_core_SOURCE_DIR}/cmsis-core
    )
