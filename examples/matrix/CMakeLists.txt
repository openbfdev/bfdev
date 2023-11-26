# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(matrix-fibonacci fibonacci.c)
target_link_libraries(matrix-fibonacci bfdev)
add_test(matrix-fibonacci matrix-fibonacci)

add_executable(matrix-benchmark benchmark.c)
target_link_libraries(matrix-benchmark bfdev)
add_test(matrix-benchmark matrix-benchmark)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        fibonacci.c
        benchmark.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/matrix
    )

    install(TARGETS
        matrix-fibonacci
        matrix-benchmark
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
