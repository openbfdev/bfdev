# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(json-selftest selftest.c)
target_link_libraries(json-selftest bfdev)
add_test(json-selftest json-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/json
    )

    install(TARGETS
        json-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
