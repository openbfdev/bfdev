# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(ringbuf-simple simple.c)
add_test(ringbuf-simple ringbuf-simple)

target_link_libraries(
    ringbuf-simple
    -Wl,--whole-archive bfdev
    -Wl,--no-whole-archive
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/ringbuf
    )

    install(TARGETS
        ringbuf-simple
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
