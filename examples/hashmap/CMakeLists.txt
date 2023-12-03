# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(hashmap-simple simple.c)
target_link_libraries(hashmap-simple bfdev)
add_test(hashmap-simple hashmap-simple)

add_executable(hashmap-benchmark benchmark.c)
target_link_libraries(hashmap-benchmark bfdev)
add_test(hashmap-benchmark hashmap-benchmark)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        benchmark.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/hashmap
    )

    install(TARGETS
        hashmap-simple
        hashmap-benchmark
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
