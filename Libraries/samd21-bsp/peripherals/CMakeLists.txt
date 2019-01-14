# Copyright Patrick Rohr, 2018

add_library(samd21_peripherals STATIC)

target_sources(samd21_peripherals PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/clock.c
    ${CMAKE_CURRENT_LIST_DIR}/src/gclk.c
)

target_include_directories(samd21_peripherals PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(samd21_peripherals PRIVATE cmsis_samd21)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmakeconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
