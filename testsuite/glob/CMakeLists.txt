# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
#

add_executable(glob-selftest selftest.c)
target_link_libraries(glob-selftest bfdev testsuite)
add_test(glob-selftest glob-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        glob-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
