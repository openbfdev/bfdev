# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(slist-iterator iterator.c)
target_link_libraries(slist-iterator bfdev testsuite)
add_test(slist-iterator slist-iterator)
