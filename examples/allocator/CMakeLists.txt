# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(allocator-simple simple.c)
target_link_libraries(allocator-simple bfdev)
add_test(allocator-simple allocator-simple)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/allocator
    )

    install(TARGETS
        allocator-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
