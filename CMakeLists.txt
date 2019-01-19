# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(samd21 C)

include(Configurations/ProjectDefinitions.cmake)

# Libraries
add_subdirectory(Libraries/BufferPool)
add_subdirectory(Libraries/cmsis-core)
add_subdirectory(Libraries/cmsis-core-samcortexm0p-samd21)
add_subdirectory(Libraries/samd21-bsp)

# Project
add_subdirectory(Project)
