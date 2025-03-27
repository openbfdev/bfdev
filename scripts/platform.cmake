# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

if(HOST_C_COMPILER)
    set(PRINT_TYPE_LINK "-static")
endif()

macro(print_type output name)
    message(STATUS "Print type: " ${name})
    try_run(
        RUN_RESULT
        COMPILE_RESULT
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/get-type.c
        COMPILE_DEFINITIONS "-Werror -DTYPE=${name}"
        LINK_OPTIONS "${PRINT_TYPE_LINK}"
        RUN_OUTPUT_VARIABLE ${output}
        COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT
    )
    if ((NOT COMPILE_RESULT) OR (NOT RUN_RESULT EQUAL 0))
        message("Compile output: ${COMPILE_OUTPUT}")
        message("Run result: ${RUN_RESULT}")
        message(FATAL_ERROR "Failed to retrieve the original type ${name}")
    endif()
endmacro()

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Darwin|GNU")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "MSYS|CYGWIN")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD|FreeBSD|NetBSD|OpenBSD|DragonFly")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Generic")
    set(BFDEV_PORT_TYPE "generic")
endif()

if(BFDEV_PORT_TYPE STREQUAL "posix")
    print_type(BFDEV_TYPE_ADDR "size_t")
    print_type(BFDEV_TYPE_W64 "int64_t")
    print_type(BFDEV_TYPE_MAX "intmax_t")
endif()

if(NOT BFDEV_TYPE_ADDR)
    set(BFDEV_TYPE_ADDR "long")
endif()

if(NOT BFDEV_TYPE_W64)
    set(BFDEV_TYPE_W64 "long long")
endif()

if(NOT BFDEV_TYPE_MAX)
    set(BFDEV_TYPE_MAX "long long")
endif()
