# Copyrights: Patrick Rohr, 2017

cmake_minimum_required(VERSION 3.5.1)

project(BufferPool C)

add_definitions(-D__ATSAMD21G18A__)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)


set(PUBLIC_HEADERS
    BufferPool.h
    )


set(SOURCES
    src/BufferPool.c
    )

set(INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(BufferPool STATIC ${SOURCES})
target_include_directories(BufferPool PUBLIC ${INCLUDE_DIR})

#target_compile_options(BufferPool PRIVATE -O0 -mtune=cortex-m0 -mthumb -march=armv6-m)
