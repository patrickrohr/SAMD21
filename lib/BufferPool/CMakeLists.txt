# Copyright Patrick Rohr, 2017-2018

cmake_minimum_required(VERSION 3.5.1)

project(BufferPool C)

set(CMAKE_C_STANDARD 11)


set(SOURCES
    src/BufferPool.c
    )

set(PUBLIC_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    )

set(INCLUDE_DIR
    ${PUBLIC_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(BufferPool STATIC ${SOURCES})
target_include_directories(BufferPool PRIVATE ${INCLUDE_DIR})
target_include_directories(BufferPool INTERFACE ${PUBLIC_INCLUDE_DIR})

#target_compile_options(BufferPool PRIVATE -O0 -mtune=cortex-m0 -mthumb -march=armv6-m)
