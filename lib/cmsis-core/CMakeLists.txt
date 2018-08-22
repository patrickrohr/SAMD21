cmake_minimum_required(VERSION 3.9.1)

project(third_party_libs)


include(ExternalProject)

ExternalProject_Add(cmsis_core_download
    GIT_REPOSITORY https://github.com/ARMmbed/cmsis-core.git
    GIT_TAG v1.2.0
    SOURCE_DIR ${CMAKE_BINARY_DIR}/thirdparty/cmsis-core
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    LOG_DOWNLOAD ON
    )

ExternalProject_Get_Property(cmsis_core_download SOURCE_DIR)
set(cmsis_SOURCE_DIR ${SOURCE_DIR})
target_include_directories(${EXE} PRIVATE ${cmsis_SOURCE_DIR}/cmsis-core)
add_dependencies(${EXE} cmsis_core_download)