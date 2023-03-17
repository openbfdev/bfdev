# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(segtree-selftest selftest.c)
target_link_libraries(segtree-selftest bfdev)

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
