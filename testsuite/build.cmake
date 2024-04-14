# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

set(TESTSUITE_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/testsuite.h
)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_library(testsuite OBJECT ${TESTSUITE_SOURCE})
