# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(hashtbl-simple simple.c)
add_test(hashtbl-simple hashtbl-simple)

target_link_libraries(
    hashtbl-simple
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/hashtbl
    )

    install(TARGETS
        hashtbl-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
