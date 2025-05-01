# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 ffashion <helloworldffashion@gmail.com>
#

add_executable(buff-string string.c)
target_link_libraries(buff-string bfdev)
add_test(buff-string buff-string)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        string.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/buff
    )

    install(TARGETS
        buff-string
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
