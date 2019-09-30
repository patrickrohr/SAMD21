# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(samd21)

include(cmake/ProjectDefinitions.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(BUILD_TESTING "Build Unit Tests" ON)
option(BUILD_SIMULATION "Build in Simulator Mode" ON)
option(BUILD_TARGET "Build in for Target" OFF)

# Libraries
add_subdirectory(lib/BufferPool)
add_subdirectory(lib/cmsis-core)
add_subdirectory(lib/cmsis-core-samcortexm0p-samd21)
add_subdirectory(lib/common)
add_subdirectory(lib/config)
add_subdirectory(lib/hal)
add_subdirectory(lib/hal_utils)
