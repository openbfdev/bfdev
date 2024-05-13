# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(hlist-iterator iterator.c)
target_link_libraries(hlist-iterator bfdev testsuite)
add_test(hlist-iterator hlist-iterator)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        hlist-iterator
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
