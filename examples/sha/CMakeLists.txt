# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(sha-bandwidth bandwidth.c)
target_link_libraries(sha-bandwidth bfdev)
add_test(sha-bandwidth sha-bandwidth)

add_executable(sha utils.c)
target_link_libraries(sha bfdev)

add_test(sha1 sha "helloworld")
add_test(sha224 sha -l "helloworld")
add_test(sha256 sha -e "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/sha
    )

    install(TARGETS
        sha
        sha-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
