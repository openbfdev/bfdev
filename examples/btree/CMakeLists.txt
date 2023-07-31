# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(btree-benchmark benchmark.c)
add_test(btree-benchmark btree-benchmark)

add_executable(btree-selftest selftest.c)
add_test(btree-selftest btree-selftest)

target_link_libraries(
    btree-benchmark
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

target_link_libraries(
    btree-selftest
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        benchmark.c
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/btree
    )

    install(TARGETS
        btree-benchmark
        btree-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
