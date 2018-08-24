# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.9.1)

project(samd21 C)

include(Configurations/ProjectDefinitions.cmake)

# Libraries
add_subdirectory(Libraries/BufferPool)
add_subdirectory(Libraries/cmsis-core)
add_subdirectory(Libraries/cmsis-core-samcortexm0p-samd21)

# Project
add_subdirectory(Project)

if(${__ATSAMD21G18A__})
execute_process(COMMAND echo "testtttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt")
endif()