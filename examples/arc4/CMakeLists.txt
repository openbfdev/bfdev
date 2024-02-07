# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(arc4-bandwidth bandwidth.c)
target_link_libraries(arc4-bandwidth bfdev)
add_test(arc4-bandwidth arc4-bandwidth)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/arc4
    )

    install(TARGETS
        arc4-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
