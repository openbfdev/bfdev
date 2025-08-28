# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
#

add_executable(overflow-selftest selftest.c)
target_link_libraries(overflow-selftest bfdev testsuite)
add_test(overflow-selftest overflow-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        overflow-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
