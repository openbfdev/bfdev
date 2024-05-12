# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(base32-bandwidth bandwidth.c)
target_link_libraries(base32-bandwidth bfdev)
add_test(base32-bandwidth base32-bandwidth)

add_executable(base32 utils.c)
target_link_libraries(base32 bfdev)
add_test(base32 base32 "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/base32
    )

    install(TARGETS
        base32
        base32-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
