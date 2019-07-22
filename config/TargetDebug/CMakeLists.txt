# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(samd21)

include(cmake/ProjectDefinitions.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Should options be moved to gloablly included file?
option(BUILD_TESTING "Build Unit Tests" OFF)
option(BUILD_SIMULATION "Build in Simulator Mode" OFF)
option(BUILD_SIMULATION "Build in for Target" ON)

# For future reference CMake 3.13 is adding target_link_options
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/../../board/SAMD21/samd21g18a_flash.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m0 -mthumb")

# Apps
add_subdirectory(app/example-app)

# Libraries
add_subdirectory(lib/BufferPool)
add_subdirectory(lib/cmsis-core)
add_subdirectory(lib/cmsis-core-samcortexm0p-samd21)
add_subdirectory(lib/common)
add_subdirectory(lib/config)
add_subdirectory(lib/hal)
add_subdirectory(lib/hal_utils)
