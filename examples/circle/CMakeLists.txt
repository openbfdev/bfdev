# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(circle-atomic atomic.c)
target_link_libraries(circle-atomic bfdev)
add_test(circle-atomic circle-atomic)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        atomic.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/circle
    )

    install(TARGETS
        circle-atomic
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
