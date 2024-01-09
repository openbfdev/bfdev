# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(fifo-selftest selftest.c)
target_link_libraries(fifo-selftest bfdev)
add_test(fifo-selftest fifo-selftest)

add_executable(fifo-atomic atomic.c)
target_link_libraries(fifo-atomic bfdev pthread)
add_test(fifo-atomic fifo-atomic)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        selftest.c
        atomic.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/fifo
    )

    install(TARGETS
        fifo-selftest
        fifo-atomic
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
