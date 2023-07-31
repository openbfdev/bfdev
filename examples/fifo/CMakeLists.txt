# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(fifo-selftest selftest.c)
add_test(fifo-selftest fifo-selftest)

add_executable(fifo-simple simple.c)
add_test(fifo-simple fifo-simple)

target_link_libraries(
    fifo-selftest
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

target_link_libraries(
    fifo-simple
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/fifo
    )

    install(TARGETS
        fifo-selftest
        fifo-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
