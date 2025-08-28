# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
#

add_executable(libc-xprintf xprintf.c)
target_link_libraries(libc-xprintf bfdev testsuite)
add_test(libc-xprintf libc-xprintf)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        libc-xprintf
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
