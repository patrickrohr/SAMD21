cmake_minimum_required(VERSION 3.11)

find_package(GoogleTest REQUIRED)

add_executable(hal_utils_test)

target_sources(hal_utils_test
    PRIVATE
        src/io_port_test.cpp
        src/main.cpp
        src/register_test.cpp
)

target_link_libraries(hal_utils_test
    PUBLIC
        samd21_hal_utils
        gtest
)

gtest_add_tests(TARGET hal_utils_test)
