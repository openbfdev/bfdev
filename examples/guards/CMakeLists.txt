# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(guards-cleanup cleanup.c)
target_link_libraries(guards-cleanup bfdev)
add_test(guards-cleanup guards-cleanup)

add_executable(guards-class class.c)
target_link_libraries(guards-class bfdev)
add_test(guards-class guards-class)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        cleanup.c
        class.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/guards
    )

    install(TARGETS
        guards-cleanup
        guards-class
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
