# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(hashmap-simple simple.c)
target_link_libraries(hashmap-simple bfdev)
add_test(hashmap-simple hashmap-simple)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/hashmap
    )

    install(TARGETS
        hashmap-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
