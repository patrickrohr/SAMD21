# Copyright Patrick Rohr, 2018

cmake_minimum_required(VERSION 3.11)

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

# if (${TOOLCHAIN} STREQUAL "Target")

#     # install(TARGETS app DESTINATION ${CMAKE_CURRENT_LIST_DIR})
#     add_custom_target(jlinkflash
#         COMMAND JLinkExe -if swd -device ATSAMD21G18 -speed 4000 -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/flashbin.jlink
#         DEPENDS app
#         )

    

# endif()