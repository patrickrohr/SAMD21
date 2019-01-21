#/**************************************************************
# *                                                            *
# *               Copyright (c) 2019 - Patrick Rohr            *
# *                      All Rights Reserved                   *
# *                                                            *
# *************************************************************/

# CMake Find Module FreeRTOS based on Amazon's FreeRTOS repository on Github:
# https://github.com/aws/amazon-freertos
cmake_minimum_required(VERSION 3.0.2)

################################################################
# Input Variables
################################################################
# Heap Memory Allocation Implementation, see https://www.freertos.org/a00111.html
if(NOT DEFINED FreeRTOS_MEMORY_ALLOC_IMPL)
    # Picking simplest one by default
    set(FreeRTOS_MEMORY_ALLOC_IMPL 1)
endif()

if(NOT DEFINED FreeRTOS_CONFIG_DIR)
    # TODO: Instead of providing this default I could just fail...
    set(FreeRTOS_CONFIG_DIR ${CMAKE_BINARY_DIR})
endif()

if(NOT DEFINED FreeRTOS_COMPILER)
    set(FreeRTOS_COMPILER "GCC")
endif()

if(NOT DEFINED FreeRTOS_ARCHITECTURE)
    set(FreeRTOS_ARCHITECTURE "ARM_CM0")
endif()


################################################################
# Download Project From Github
################################################################
include(FetchContent)

FetchContent_Declare(freertos_download
    GIT_REPOSITORY https://github.com/aws/amazon-freertos.git
    GIT_TAG        v1.2.0
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(freertos_download)

if (NOT freertos_download_POPULATED)
    FetchContent_Populate(freertos_download)

    add_library(FreeRTOS STATIC)

    target_sources(FreeRTOS 
        PUBLIC
            ${freertos_download_SOURCE_DIR}/lib/include/FreeRTOS.h
        PRIVATE
            # required
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/tasks.c
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/list.c
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/queue.c
            # optional
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/event_groups.c
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/stream_buffer.c
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/timers.c
            # Portable
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/portable/${FreeRTOS_COMPILER}/${FreeRTOS_ARCHITECTURE}/port.c
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/portable/${FreeRTOS_COMPILER}/${FreeRTOS_ARCHITECTURE}/portmacro.h
            # MemMang
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/portable/MemMang/heap_${FreeRTOS_MEMORY_ALLOC_IMPL}.c
    )

    target_include_directories(FreeRTOS
        PRIVATE
            ${FreeRTOS_CONFIG_DIR}
            ${freertos_download_SOURCE_DIR}/lib/include/private
            ${freertos_download_SOURCE_DIR}/lib/FreeRTOS/portable/${FreeRTOS_COMPILER}/${FreeRTOS_ARCHITECTURE}/
        PUBLIC
            ${freertos_download_SOURCE_DIR}/lib/include/
    )
endif()

find_package_handle_standard_args(FreeRTOS
    FOUND_VAR FreeRTOS_FOUND
    REQUIRED_VARS freertos_download_SOURCE_DIR freertos_download_POPULATED
    # TODO: Versioning
)

