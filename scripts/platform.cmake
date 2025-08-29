# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
#

if(CMAKE_SYSTEM_NAME MATCHES "Linux|Android|Darwin|GNU")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows|MSYS|CYGWIN")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD|FreeBSD|NetBSD|OpenBSD|DragonFly")
    set(BFDEV_PORT_TYPE "posix")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Generic")
    set(BFDEV_PORT_TYPE "generic")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/get-type.cmake)
get_type(BFDEV_TYPE_ADDR "size_t" "long")
get_type(BFDEV_TYPE_W64 "int64_t" "long long")
get_type(BFDEV_TYPE_MAX "intmax_t" "long long")
