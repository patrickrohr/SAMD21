
set(CMAKE_VERBOSE_MAKEFILE on)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 14)

# if(TOOLCHAIN STREQUAL "Target")
# endif()

option(BUILD_TESTING "Build Unit Tests")

# Project specific compile flags
add_compile_options(-D__ATSAMD21G18A__)

# Generic compile flags
add_compile_options(-O0)
add_compile_options(-Wall)

# Functions
function(add_test_directory dir)
    if(BUILD_TESTING)
        add_subdirectory(${dir})
    endif()
endfunction()

function(samd_target_install_binary target install_dir)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary -S ${CMAKE_CURRENT_BINARY_DIR}/${target} ${install_dir}/${target}.bin
    )
endfunction()
