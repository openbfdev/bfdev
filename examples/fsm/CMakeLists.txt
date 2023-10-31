# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(fsm-console console.c)
target_link_libraries(fsm-console bfdev)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        console.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/fsm
    )

    install(TARGETS
        fsm-console
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
