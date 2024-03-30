# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(list-iterator iterator.c)
target_link_libraries(list-iterator bfdev testsuite)
add_test(list-iterator list-iterator)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        list-iterator
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
