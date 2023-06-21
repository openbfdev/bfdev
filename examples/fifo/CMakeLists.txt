# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(fifo-selftest selftest.c)
target_link_libraries(fifo-selftest bfdev)
add_test(fifo-selftest fifo-selftest)

add_executable(fifo-simple simple.c)
target_link_libraries(fifo-simple bfdev)
add_test(fifo-simple fifo-simple)

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
