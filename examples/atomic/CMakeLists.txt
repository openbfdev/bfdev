# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(atomic-spinlock spinlock.c)
target_link_libraries(atomic-spinlock bfdev)
add_test(atomic-spinlock atomic-spinlock)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        spinlock.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/atomic
    )

    install(TARGETS
        atomic-spinlock
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
