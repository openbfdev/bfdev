# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR})

get_property(BFDEV_INCLUDE_DIRS GLOBAL PROPERTY "BFDEV_INCLUDE_DIRS")
include_directories(${BFDEV_INCLUDE_DIRS})
