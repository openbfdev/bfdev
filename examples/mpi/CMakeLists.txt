# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(mpi-bbp bbp.c)
target_link_libraries(mpi-bbp bfdev)
add_test(mpi-bbp mpi-bbp)

add_executable(mpi-marchin marchin.c)
target_link_libraries(mpi-marchin bfdev)
add_test(mpi-marchin mpi-marchin)

add_executable(mpi-fibonacci fibonacci.c)
target_link_libraries(mpi-fibonacci bfdev)
add_test(mpi-fibonacci mpi-fibonacci)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bbp.c
        marchin.c
        fibonacci.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/mpi
    )

    install(TARGETS
        mpi-bbp
        mpi-marchin
        mpi-fibonacci
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
