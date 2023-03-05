# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

cmake_minimum_required (VERSION 3.25.2)
project (bfdev C)
include (GNUInstallDirs)

set(bfdev_HEADERS
    include/bfdev/attributes.h
    include/bfdev/bitrev.h
    include/bfdev/bsearch.h
    include/bfdev/compiler.h
    include/bfdev/const.h
    include/bfdev/container.h
    include/bfdev/list.h
    include/bfdev/poison.h
    include/bfdev/rbtree.h
    include/bfdev/scnprintf.h
    include/bfdev/stringify.h
)

set(bfdev_SRCS
    ${bfdev_HEADERS}
    src/bitrev.c
    src/bsearch.c
    src/list-check.c
    src/list-sort.c
    src/rbtree.c
    src/rbtree-debug.c
    src/scnprintf.c
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(bfdev_shared SHARED ${bfdev_SRCS})
add_library(bfdev_static STATIC ${bfdev_SRCS})

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

install(FILES ${bfdev_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bfdev")

install(FILES
    docs/logo.svg
    DESTINATION
    "${CMAKE_INSTALL_DOCDIR}"
)

install(TARGETS bfdev_static bfdev_shared
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


add_library(bfdev ALIAS bfdev_shared)
add_subdirectory(examples)
