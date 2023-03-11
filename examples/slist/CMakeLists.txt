# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(slist-selftest selftest.c)
target_link_libraries(slist-selftest bfdev)

add_executable(slist-simple simple.c)
target_link_libraries(slist-simple bfdev)

install(FILES
    selftest.c
    simple.c
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/examples/slist
)

install(TARGETS
    slist-selftest
    slist-simple
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/bin
)
