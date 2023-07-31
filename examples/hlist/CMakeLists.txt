# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(hlist-selftest selftest.c)
add_test(hlist-selftest hlist-selftest)

add_executable(hlist-simple simple.c)
add_test(hlist-simple hlist-simple)

target_link_libraries(
    hlist-selftest
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

target_link_libraries(
    hlist-simple
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/hlist
    )

    install(TARGETS
        hlist-selftest
        hlist-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
