# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(sort-selftest selftest.c)
target_link_libraries(sort-selftest bfdev)
add_test(sort-selftest sort-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/sort
    )

    install(TARGETS
        sort-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
