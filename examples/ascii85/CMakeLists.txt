# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

add_executable(ascii85-bandwidth bandwidth.c)
target_link_libraries(ascii85-bandwidth bfdev)
add_test(ascii85-bandwidth ascii85-bandwidth)

add_executable(ascii85 utils.c)
target_link_libraries(ascii85 bfdev)
add_test(ascii85 ascii85 "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/ascii85
    )

    install(TARGETS
        ascii85
        ascii85-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
