# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(array-simple-append simple-append.c)
target_link_libraries(array-simple-append bfdev)
add_test(array-simple-append array-simple-append)

add_executable(array-simple-splice simple-splice.c)
target_link_libraries(array-simple-splice bfdev)
add_test(array-simple-splice array-simple-splice)

add_executable(array-simple-stack simple-stack.c)
target_link_libraries(array-simple-stack bfdev)
add_test(array-simple-stack array-simple-stack)

add_executable(array-simple-write simple-write.c)
target_link_libraries(array-simple-write bfdev)
add_test(array-simple-write array-simple-write)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple-append.c
        simple-splice.c
        simple-stack.c
        simple-write.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/array
    )

    install(TARGETS
        array-simple-append
        array-simple-splice
        array-simple-stack
        array-simple-write
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
