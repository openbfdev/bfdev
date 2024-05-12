# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(base64-bandwidth bandwidth.c)
target_link_libraries(base64-bandwidth bfdev)
add_test(base64-bandwidth base64-bandwidth)

add_executable(base64 utils.c)
target_link_libraries(base64 bfdev)
add_test(base64 base64 "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/base64
    )

    install(TARGETS
        base64
        base64-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
