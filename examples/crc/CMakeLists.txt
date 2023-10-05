# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(crc-bandwidth bandwidth.c)
target_link_libraries(crc-bandwidth bfdev)
add_test(crc-bandwidth crc-bandwidth)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/bfdev
    )

    install(TARGETS
        crc-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
