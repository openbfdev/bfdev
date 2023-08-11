# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(radix-simple simple.c)
target_link_libraries(radix-simple bfdev)
add_test(radix-simple radix-simple)

add_executable(radix-benchmark benchmark.c)
target_link_libraries(radix-benchmark bfdev)
add_test(radix-benchmark radix-benchmark)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        benchmark.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/radix
    )

    install(TARGETS
        radix-simple
        radix-benchmark
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
