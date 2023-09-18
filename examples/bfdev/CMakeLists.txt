# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(bfdev-version version.c)
target_link_libraries(bfdev-version bfdev)
add_test(bfdev-version bfdev-version)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        version.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/bfdev
    )

    install(TARGETS
        bfdev-version
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
