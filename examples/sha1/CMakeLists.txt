# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(sha1-bandwidth bandwidth.c)
target_link_libraries(sha1-bandwidth bfdev)
add_test(sha1-bandwidth sha1-bandwidth)

add_executable(sha1 utils.c)
target_link_libraries(sha1 bfdev)
add_test(sha1 sha1 "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/sha1
    )

    install(TARGETS
        sha1
        sha1-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
