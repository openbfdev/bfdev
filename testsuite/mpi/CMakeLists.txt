# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(mpi-fuzzy fuzzy.c)
target_link_libraries(mpi-fuzzy bfdev testsuite)
add_test(mpi-fuzzy mpi-fuzzy)

add_executable(mpi-empty empty.c)
target_link_libraries(mpi-empty bfdev testsuite)
add_test(mpi-empty mpi-empty)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        mpi-fuzzy
        mpi-empty
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
