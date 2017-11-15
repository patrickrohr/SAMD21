# Copyrights: Patrick Rohr, 2017

cmake_minimum_required(VERSION 3.5.1)

project(samd21 C)

add_definitions(-D__ATSAMD21G18A__)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/vendor/SAMD21/samd21g18a_flash.ld")



set(SOURCES
    src/main.c
    # Hardware Access Layer
    src/hal/PortIO.c
    src/hal/UART.c
    # Helpers
    src/helpers/BufferPool.c 
    src/helpers/Error.c 
    src/helpers/HDLCPacket.c 
    src/helpers/PacketManager.c 
    src/helpers/TextPacket.c 
    # Interrupt Routines
    src/irq/SercomHandler.c
    # Tasks 
    src/tasks/TaskRX.c 
    src/tasks/TaskTX.c
    # 3rd Party Software
    #vendor/SAMD21/startup_samd21.c 
    #vendor/SAMD21/system_samd21.c 
    vendor/FreeRTOS/Source/tasks.c
    vendor/FreeRTOS/Source/queue.c
    vendor/FreeRTOS/Source/list.c
    vendor/FreeRTOS/Source/timers.c
    vendor/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
    vendor/FreeRTOS/Source/portable/MemMang/heap_2.c
    # CMSIS
    vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis_nvic.c
    vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/source/system_samd21.c
    vendor/cmsis-core-atmel-samcortexm0p-samd21/source/TARGET_SAMD21G18A/TOOLCHAIN_GCC_ARM/startup_samd21.c
    # CMSIS cmsis-core 
    #vendor/cmsis-core/source/cmsis_nvic.c
    )

set(INCLUDE_DIR
    # FreeRTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/FreeRTOS/Source/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/FreeRTOS/Source/portable/GCC/ARM_CM0/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hal/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/irq/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tasks/
    # CMSIS - TODO: build vendor libraries independently and link.
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/TARGET_SAMD21G18A/TOOLCHAIN_GCC_ARM/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/source/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/include/component/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/include/instance/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/cmsis/TARGET_SAMD21/include/pio/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/header_files/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core-atmel-samcortexm0p-samd21/source/preprocessor/
    # CMSIS Core 
    #${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis-core/cmsis-core/
    )

add_executable(samd21 ${SOURCES})
include_directories(${INCLUDE_DIR})

target_compile_options(samd21 PRIVATE -O0 -mtune=cortex-m0 -mthumb -march=armv6-m)