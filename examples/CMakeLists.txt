# SPDX-License-Identifier: MIT
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(list-benchmark list-benchmark.c)
set_source_files_properties(list-benchmark
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")
target_link_libraries(list-benchmark bfdev)

add_executable(list-selftest list-selftest.c)
set_source_files_properties(list-selftest
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")
target_link_libraries(list-selftest bfdev)

install(FILES list-benchmark.c list-selftest.c
        DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples)

install(TARGETS list-benchmark list-selftest DESTINATION ${CMAKE_INSTALL_BINDIR})
