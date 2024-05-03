# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(memalloc-fuzzy fuzzy.c)
target_link_libraries(memalloc-fuzzy bfdev testsuite)
add_test(memalloc-fuzzy memalloc-fuzzy)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(TARGETS
        memalloc-fuzzy
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/testsuite
    )
endif()
