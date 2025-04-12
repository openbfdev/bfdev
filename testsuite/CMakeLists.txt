# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

include(${CMAKE_CURRENT_LIST_DIR}/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/testsuite.cmake)

add_subdirectory(array)
add_subdirectory(bitwalk)
add_subdirectory(fifo)
add_subdirectory(glob)
add_subdirectory(hlist)
add_subdirectory(libc)
add_subdirectory(list)
add_subdirectory(memalloc)
add_subdirectory(mpi)
add_subdirectory(slist)
