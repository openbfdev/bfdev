# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

cmake_minimum_required(VERSION 3.6)
project(bfdev LANGUAGES C)
include(GNUInstallDirs)

set(BFDEV_HEADERS
    include/bfdev/attributes.h
    include/bfdev/bcd.h
    include/bfdev/bitrev.h
    include/bfdev/bsearch.h
    include/bfdev/compiler.h
    include/bfdev/const.h
    include/bfdev/container.h
    include/bfdev/list.h
    include/bfdev/once.h
    include/bfdev/overflow.h
    include/bfdev/poison.h
    include/bfdev/popcount.h
    include/bfdev/rbtree.h
    include/bfdev/scnprintf.h
    include/bfdev/skiplist.h
    include/bfdev/stringify.h
)

set(BFDEV_SRCS
    src/bcd.c
    src/bitrev.c
    src/bsearch.c
    src/list-sort.c
    src/popcount.c
    src/rbtree.c
    src/scnprintf.c
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

option(ENABLE_DBGLIST "Dynamic debug list" OFF)
if(ENABLE_DBGLIST)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -D DEBUG_LIST"
    )
    set(BFDEV_SRCS
        src/list-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGLIST)

option(ENABLE_DBGRBTREE "Dynamic debug rbtree" OFF)
if(ENABLE_DBGRBTREE)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -D DEBUG_RBTREE"
    )
    set(BFDEV_SRCS
        src/rbtree-debug.c
        ${BFDEV_SRCS}
    )
endif(ENABLE_DBGRBTREE)

include_directories(${PROJECT_SOURCE_DIR}/include)
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
