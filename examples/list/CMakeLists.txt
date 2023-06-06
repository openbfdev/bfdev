# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(list-benchmark benchmark.c)
target_link_libraries(list-benchmark bfdev)
add_test(list-benchmark list-benchmark)

add_executable(list-selftest selftest.c)
target_link_libraries(list-selftest bfdev)
add_test(list-selftest list-selftest)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        benchmark.c
        selftest.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/list
    )

    install(TARGETS
        list-benchmark
        list-selftest
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
