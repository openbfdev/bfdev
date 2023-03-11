# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(rbtree-benchmark benchmark.c)
target_link_libraries(rbtree-benchmark bfdev)

add_executable(rbtree-selftest selftest.c)
target_link_libraries(rbtree-selftest bfdev)

add_executable(rbtree-simple selftest.c)
target_link_libraries(rbtree-simple bfdev)

install(FILES
    benchmark.c
    selftest.c
    simple.c
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/examples/rbtree
)

install(TARGETS
    rbtree-benchmark
    rbtree-selftest
    rbtree-simple
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/bin
)
