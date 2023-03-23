# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(hlist-selftest selftest.c)
target_link_libraries(hlist-selftest bfdev)

add_executable(hlist-simple simple.c)
target_link_libraries(hlist-simple bfdev)

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
