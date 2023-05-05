# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.6)
project(bfdev LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)
include(CheckCXXSymbolExists)
include(CheckFunctionExists)
include(CheckCSourceCompiles)
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake
)

set(BFDEV_HEADERS
    include/bfdev/align.h
    include/bfdev/allocator.h
    include/bfdev/attributes.h
    include/bfdev/array.h
    include/bfdev/bcd.h
    include/bfdev/bitrev.h
    include/bfdev/bits.h
    include/bfdev/bitsperlong.h
    include/bfdev/bsearch.h
    include/bfdev/compiler.h
    include/bfdev/const.h
    include/bfdev/container.h
    include/bfdev/errno.h
    include/bfdev/galloctor.h
    include/bfdev/hash.h
    include/bfdev/hlist.h
    include/bfdev/limits.h
    include/bfdev/list.h
    include/bfdev/macro.h
    include/bfdev/mhelper.h
    include/bfdev/minmax.h
    include/bfdev/once.h
    include/bfdev/overflow.h
    include/bfdev/poison.h
    include/bfdev/popcount.h
    include/bfdev/rbtree.h
    include/bfdev/scnprintf.h
    include/bfdev/segtree.h
    include/bfdev/skiplist.h
    include/bfdev/slist.h
    include/bfdev/stdarg.h
    include/bfdev/stdbool.h
    include/bfdev/stddef.h
    include/bfdev/stdint.h
    include/bfdev/stdio.h
    include/bfdev/stdlib.h
    include/bfdev/string.h
    include/bfdev/stringify.h
    include/bfdev/swab.h
    include/bfdev/typecheck.h
)

set(BFDEV_SRCS
    src/array.c
    src/bcd.c
    src/bitrev.c
    src/bsearch.c
    src/errname.c
    src/gallocator.c
    src/list-sort.c
    src/popcount.c
    src/rbtree.c
    src/scnprintf.c
    src/segtree.c
    src/skiplist.c
    ${BFDEV_HEADERS}
)

add_definitions(
    "-std=gnu17 "
    "-Wall "
    "-Wextra "
    "-Wno-override-init "
    "-Wno-unused-parameter "
    "-Wno-sign-compare "
    "-Wno-pointer-sign "
    "-Wno-null-pointer-arithmetic "
    "-fvisibility=hidden "
)

option(ENABLE_STRICT "Enable strict compilation" ON)
if(ENABLE_STRICT)
    add_definitions(
        "-Werror "
    )
endif(ENABLE_STRICT)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=address -fsanitize=undefined \
         -fsanitize=leak -fsanitize-recover=all \
         -fno-omit-frame-pointer -fno-stack-protector"
    )
endif(ENABLE_ASAN)

option(ENABLE_UBSAN "Enable Undefined Behaviour Sanitizer" OFF)
if(ENABLE_UBSAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=alignment \
         -fsanitize=bounds \
         -fsanitize=shift \
         -fsanitize=integer-divide-by-zero \
         -fsanitize=unreachable \
         -fsanitize=bool \
         -fsanitize=enum \
         -fsanitize-undefined-trap-on-error"
    )
endif(ENABLE_UBSAN)

option(ENABLE_DBGLIST "Dynamic debug list" OFF)
if(ENABLE_DBGLIST)
    set(BFDEV_DEBUG_LIST ON)
    set(BFDEV_SRCS
        src/list-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGLIST)

option(ENABLE_DBGSLIST "Dynamic debug slist" OFF)
if(ENABLE_DBGSLIST)
    set(BFDEV_DEBUG_SLIST ON)
    set(BFDEV_SRCS
        src/slist-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGSLIST)

option(ENABLE_DBGHLIST "Dynamic debug hlist" OFF)
if(ENABLE_DBGHLIST)
    set(BFDEV_DEBUG_HLIST ON)
    set(BFDEV_SRCS
        src/hlist-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGHLIST)

option(ENABLE_DBGRBTREE "Dynamic debug rbtree" OFF)
if(ENABLE_DBGRBTREE)
    set(BFDEV_DEBUG_RBTREE ON)
    set(BFDEV_SRCS
        src/rbtree-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGRBTREE)


CHECK_C_SOURCE_COMPILES(
"
int main(void) {
    long  n = 0;

    if (!__sync_bool_compare_and_swap(&n, 0, 3))
        return 1;
    if (__sync_fetch_and_add(&n, 1) != 3)
        return 1;
    if (__sync_lock_test_and_set(&n, 5) != 4)
        return 1;
    if (n != 5)
        return 1;
    if (__sync_or_and_fetch(&n, 2) != 7)
        return 1;
    if (__sync_and_and_fetch(&n, 5) != 5)
        return 1;
    __sync_lock_release(&n);
    if (n != 0)
        return 1;
    return 0;
}

"
BFDEV_HAVE_GCC_ATOMIC)


configure_file(
    ${CMAKE_MODULE_PATH}/config.h.in
    ${PROJECT_BINARY_DIR}/generated/bfdev/config.h
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/generated)

add_library(bfdev_shared SHARED ${BFDEV_SRCS})
add_library(bfdev_static STATIC ${BFDEV_SRCS})

SET_TARGET_PROPERTIES(bfdev_shared
    PROPERTIES VERSION 1.0 SOVERSION 1
    OUTPUT_NAME bfdev
    MACOSX_RPATH ON
    LINKER_LANGUAGE C
)

SET_TARGET_PROPERTIES(bfdev_static
    PROPERTIES
    OUTPUT_NAME bfdev
    LINKER_LANGUAGE C
)

install(FILES
    ${BFDEV_HEADERS}
    DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/bfdev"
)

install(FILES
    ${PROJECT_BINARY_DIR}/generated/bfdev/config.h
    DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/bfdev"
)

install(FILES
    docs/logo.svg
    DESTINATION
    "${CMAKE_INSTALL_DOCDIR}"
)

install(TARGETS
    bfdev_static bfdev_shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_library(bfdev ALIAS bfdev_shared)
add_subdirectory(examples)
