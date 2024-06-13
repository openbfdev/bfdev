# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(sha1 utils.c)
target_link_libraries(sha1 bfdev)
add_test(sha1 sha1 "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/sha1
    )

    install(TARGETS
        sha1
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
