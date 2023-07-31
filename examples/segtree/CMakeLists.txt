# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(segtree-selftest selftest.c)
add_test(segtree-selftest segtree-selftest)

target_link_libraries(
    segtree-selftest
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/segtree
    )

    install(TARGETS
        segtree-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
