# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(overflow-simple simple.c)
target_link_libraries(overflow-simple bfdev)
add_test(overflow-simple overflow-simple)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/overflow
    )

    install(TARGETS
        overflow-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
