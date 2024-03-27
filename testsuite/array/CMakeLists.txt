# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

testsuite_target(array-fuzzy
    ${PROJECT_SOURCE_DIR}/src/array.c
    ${CMAKE_CURRENT_LIST_DIR}/fuzzy.c
)

add_test(array-fuzzy array-fuzzy)
