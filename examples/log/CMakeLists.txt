# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(log-simple simple.c)
target_link_libraries(log-simple bfdev)
add_test(log-simple log-simple)

add_executable(log-date date.c)
target_link_libraries(log-date bfdev)
add_test(log-date log-date)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        date.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/log
    )

    install(TARGETS
        log-simple
        log-date
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
