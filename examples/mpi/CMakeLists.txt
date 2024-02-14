# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(mpi-marchin marchin.c)
target_link_libraries(mpi-marchin bfdev)
add_test(mpi-marchin mpi-marchin)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        marchin.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/mpi
    )

    install(TARGETS
        mpi-marchin
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
