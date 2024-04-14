# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(list-benchmark benchmark.c)
target_link_libraries(list-benchmark bfdev)
add_test(list-benchmark list-benchmark)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        benchmark.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/list
    )

    install(TARGETS
        list-benchmark
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
