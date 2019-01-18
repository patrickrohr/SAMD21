# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

project(samd21 C)

include(Configurations/ProjectDefinitions.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(Kconfiglib_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/Libraries/samd21-bsp/config")
set(Kconfiglib_CONFIG_FILENAME "Config.in")
set(Kconfiglib_HEADER_OUTPUT "${CMAKE_BINARY_DIR}")
find_package(Kconfiglib REQUIRED)

# Libraries
add_subdirectory(Libraries/BufferPool)
add_subdirectory(Libraries/cmsis-core)
add_subdirectory(Libraries/cmsis-core-samcortexm0p-samd21)
add_subdirectory(Libraries/samd21-bsp)

# Project
add_subdirectory(Project)

if(${__ATSAMD21G18A__})
    execute_process(COMMAND echo "testtttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt")
endif()

# if (${TOOLCHAIN} STREQUAL "Target")

#     # install(TARGETS app DESTINATION ${CMAKE_CURRENT_LIST_DIR})
#     add_custom_target(jlinkflash
#         COMMAND JLinkExe -if swd -device ATSAMD21G18 -speed 4000 -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/flashbin.jlink
#         DEPENDS app
#         )

    

# endif()
