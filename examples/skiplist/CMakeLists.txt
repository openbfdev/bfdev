# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(skiplist-benchmark benchmark.c)
target_link_libraries(skiplist-benchmark bfdev)
add_test(skiplist-benchmark skiplist-benchmark)

add_executable(skiplist-selftest selftest.c)
target_link_libraries(skiplist-selftest bfdev)
add_test(skiplist-selftest skiplist-selftest)

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
