# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
#

add_executable(log-simple simple.c)
target_link_libraries(log-simple bfdev)
add_test(log-simple log-simple)

add_executable(log-modules modules.c)
target_link_libraries(log-modules bfdev)
add_test(log-modules log-modules)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        simple.c
        modules.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/log
    )

    install(TARGETS
        log-simple
        log-modules
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
