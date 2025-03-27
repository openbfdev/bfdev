# SPDX-License-Identifier: LGPL-3.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

if(BFDEV_PORT_TYPE STREQUAL "posix")
    add_executable(bfdev-typecheck ${CMAKE_CURRENT_LIST_DIR}/typecheck.c)
    target_compile_options(bfdev-typecheck PRIVATE -Werror)
    target_link_libraries(bfdev-typecheck bfdev)
endif()
