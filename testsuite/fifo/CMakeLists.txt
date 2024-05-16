# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(fifo-selftest selftest.c)
target_link_libraries(fifo-selftest bfdev testsuite)
add_test(fifo-selftest fifo-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        fifo-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
