# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(tokenbucket-simple simple.c)
target_link_libraries(tokenbucket-simple bfdev)
add_test(tokenbucket-simple tokenbucket-simple)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/tokenbucket
    )

    install(TARGETS
        tokenbucket-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
