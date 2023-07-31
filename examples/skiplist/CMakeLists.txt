# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(skiplist-benchmark benchmark.c)
add_test(skiplist-benchmark skiplist-benchmark)

add_executable(skiplist-selftest selftest.c)
add_test(skiplist-selftest skiplist-selftest)

target_link_libraries(
    skiplist-benchmark
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

target_link_libraries(
    skiplist-selftest
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        benchmark.c
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/skiplist
    )

    install(TARGETS
        skiplist-benchmark
        skiplist-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
