# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(heap-benchmark benchmark.c)
target_link_libraries(heap-benchmark bfdev)
add_test(heap-benchmark heap-benchmark)

add_executable(heap-selftest selftest.c)
target_link_libraries(heap-selftest bfdev)
add_test(heap-selftest heap-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        benchmark.c
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/heap
    )

    install(TARGETS
        heap-benchmark
        heap-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
