# SPDX-License-Identifier: LGPL-3.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

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

if(BFDEV_PORT_TYPE)
    include(${BFDEV_PORT_PATH}/${BFDEV_PORT_TYPE}/build.cmake)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()
