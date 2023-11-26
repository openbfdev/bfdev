# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(guards-cleanup cleanup.c)
target_link_libraries(guards-cleanup bfdev)
add_test(guards-cleanup guards-cleanup)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        cleanup.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/guards
    )

    install(TARGETS
        guards-cleanup
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
