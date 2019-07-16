# /**************************************************************
#  *                                                            *
#  *               Copyright (c) 2019 - Patrick Rohr            *
#  *                      All Rights Reserved                   *
#  *                                                            *
#  *************************************************************/

cmake_minimum_required(VERSION 3.11)

################################################################
# Board Support Package
################################################################
set(Kconfiglib_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config")
set(Kconfiglib_DOT_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config")
set(Kconfiglib_CONFIG_FILENAME "Config.in")
set(Kconfiglib_HEADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}")
find_package(Kconfiglib QUIET)

# Does not belong in here
# set(FreeRTOS_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config/FreeRTOS")
# find_package(FreeRTOS REQUIRED)
