# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(list-benchmark list/benchmark.c)
target_link_libraries(list-benchmark bfdev)
set_source_files_properties(list-benchmark
    PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter"
)

add_executable(list-selftest list/selftest.c)
target_link_libraries(list-selftest bfdev)
set_source_files_properties(list-selftest
    PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter"
)

add_executable(rbtree-benchmark rbtree/benchmark.c)
target_link_libraries(rbtree-benchmark bfdev)
set_source_files_properties(rbtree-benchmark
    PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter"
)

add_executable(rbtree-selftest rbtree/selftest.c)
target_link_libraries(rbtree-selftest bfdev)
set_source_files_properties(rbtree-selftest
    PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter"
)

add_executable(rbtree-simple rbtree/simple.c)
target_link_libraries(rbtree-simple bfdev)
set_source_files_properties(rbtree-simple
    PROPERTIES COMPILE_FLAGS
    "-Wall -Wno-unused-parameter"
)

add_executable(skiplist-benchmark skiplist/benchmark.c)
set_source_files_properties(skiplist-benchmark
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")
target_link_libraries(skiplist-benchmark bfdev)

add_executable(skiplist-selftest skiplist/selftest.c)
set_source_files_properties(skiplist-selftest
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")
target_link_libraries(skiplist-selftest bfdev)

install(FILES
    list/benchmark.c
    list/selftest.c
    rbtree/benchmark.c
    rbtree/selftest.c
    rbtree/simple.c
    skiplist/benchmark.c
    skiplist/selftest.c
    DESTINATION
    ${CMAKE_INSTALL_DOCDIR}/examples
)

install(TARGETS
    list-benchmark
    list-selftest
    rbtree-benchmark
    rbtree-selftest
    rbtree-simple
    skiplist-benchmark
    skiplist-selftest
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
