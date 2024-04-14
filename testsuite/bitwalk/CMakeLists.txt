# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(bitwalk-fuzzy fuzzy.c)
target_link_libraries(bitwalk-fuzzy bfdev testsuite)
add_test(bitwalk-fuzzy bitwalk-fuzzy)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        bitwalk-fuzzy
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
