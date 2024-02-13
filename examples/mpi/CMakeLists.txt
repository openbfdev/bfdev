# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(mpi-pi pi.c)
target_link_libraries(mpi-pi bfdev)
add_test(mpi-pi mpi-pi)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        pi.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/mpi
    )

    install(TARGETS
        mpi-pi
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
