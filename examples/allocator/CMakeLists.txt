# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
#

add_executable(allocator-simple simple.c)
target_link_libraries(allocator-simple bfdev)
add_test(allocator-simple allocator-simple)

add_executable(allocator-guards guards.c)
target_link_libraries(allocator-guards bfdev)
add_test(allocator-guards allocator-guards)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        guards.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/allocator
    )

    install(TARGETS
        allocator-simple
        allocator-guards
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
