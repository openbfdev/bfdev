# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(levenshtein-simple simple.c)
target_link_libraries(levenshtein-simple bfdev)

install(FILES
    simple.c
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/examples/levenshtein
)

install(TARGETS
    levenshtein-simple
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/bin
)
