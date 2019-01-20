# Copyright Patrick Rohr, 2018

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(Kconfiglib_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/../config")
set(Kconfiglib_DOT_CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/../config")
set(Kconfiglib_CONFIG_FILENAME "Config.in")
set(Kconfiglib_HEADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}")
find_package(Kconfiglib QUIET)

add_library(samd21_peripherals STATIC)

target_sources(samd21_peripherals PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/clock.c
    ${CMAKE_CURRENT_LIST_DIR}/src/gclk.c
)

target_include_directories(samd21_peripherals PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_include_directories(samd21_peripherals PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(samd21_peripherals PRIVATE cmsis_samd21 m)

target_compile_features(samd21_peripherals PRIVATE c_static_assert)

if(Kconfiglib_FOUND)
    add_dependencies(samd21_peripherals genconfig)
endif()
