# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(ilist-selftest selftest.c)
target_link_libraries(ilist-selftest bfdev)
add_test(ilist-selftest ilist-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/ilist
    )

    install(TARGETS
        ilist-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
