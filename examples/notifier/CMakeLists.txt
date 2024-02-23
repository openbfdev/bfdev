# SPDX-License-Identifier: GPL-2.0-or-later */
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(notifier-simple simple.c)
target_link_libraries(notifier-simple bfdev)
add_test(notifier-simple notifier-simple)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/notifier
    )

    install(TARGETS
        notifier-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
