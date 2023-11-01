# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/allocator.c
    ${CMAKE_CURRENT_LIST_DIR}/allocpool.c
    ${CMAKE_CURRENT_LIST_DIR}/argv.c
    ${CMAKE_CURRENT_LIST_DIR}/array.c
    ${CMAKE_CURRENT_LIST_DIR}/bcd.c
    ${CMAKE_CURRENT_LIST_DIR}/bitmap.c
    ${CMAKE_CURRENT_LIST_DIR}/bitrev.c
    ${CMAKE_CURRENT_LIST_DIR}/bitwalk.c
    ${CMAKE_CURRENT_LIST_DIR}/bloom.c
    ${CMAKE_CURRENT_LIST_DIR}/bsearch.c
    ${CMAKE_CURRENT_LIST_DIR}/btree.c
    ${CMAKE_CURRENT_LIST_DIR}/btree-utils.c
    ${CMAKE_CURRENT_LIST_DIR}/callback.c
    ${CMAKE_CURRENT_LIST_DIR}/errname.c
    ${CMAKE_CURRENT_LIST_DIR}/fifo.c
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
    ${CMAKE_CURRENT_LIST_DIR}/hashmap.c
    ${CMAKE_CURRENT_LIST_DIR}/heap.c
    ${CMAKE_CURRENT_LIST_DIR}/ilist.c
    ${CMAKE_CURRENT_LIST_DIR}/levenshtein.c
    ${CMAKE_CURRENT_LIST_DIR}/list-sort.c
    ${CMAKE_CURRENT_LIST_DIR}/llist.c
    ${CMAKE_CURRENT_LIST_DIR}/log.c
    ${CMAKE_CURRENT_LIST_DIR}/lru.c
    ${CMAKE_CURRENT_LIST_DIR}/matrix.c
    ${CMAKE_CURRENT_LIST_DIR}/minpool.c
    ${CMAKE_CURRENT_LIST_DIR}/notifier.c
    ${CMAKE_CURRENT_LIST_DIR}/popcount.c
    ${CMAKE_CURRENT_LIST_DIR}/prandom.c
    ${CMAKE_CURRENT_LIST_DIR}/radix.c
    ${CMAKE_CURRENT_LIST_DIR}/rbtree.c
    ${CMAKE_CURRENT_LIST_DIR}/respool.c
    ${CMAKE_CURRENT_LIST_DIR}/ringbuf.c
    ${CMAKE_CURRENT_LIST_DIR}/scnprintf.c
    ${CMAKE_CURRENT_LIST_DIR}/segtree.c
    ${CMAKE_CURRENT_LIST_DIR}/skiplist.c
    ${CMAKE_CURRENT_LIST_DIR}/stringhash.c
)

if(BFDEV_DBGLIST)
    set(BFDEV_SOURCE
        ${BFDEV_SOURCE}
        ${CMAKE_CURRENT_LIST_DIR}/list-debug.c
    )
endif()

if(BFDEV_DBGSLIST)
    set(BFDEV_SOURCE
        ${BFDEV_SOURCE}
        ${CMAKE_CURRENT_LIST_DIR}/slist-debug.c
    )
endif()

if(BFDEV_DBGHLIST)
    set(BFDEV_SOURCE
        ${BFDEV_SOURCE}
        ${CMAKE_CURRENT_LIST_DIR}/hlist-debug.c
    )
endif()

if(BFDEV_DBGRBTREE)
    set(BFDEV_SOURCE
        ${BFDEV_SOURCE}
        ${CMAKE_CURRENT_LIST_DIR}/rbtree-debug.c
    )
endif()

if(BFDEV_DBGHEAP)
    set(BFDEV_SOURCE
        ${BFDEV_SOURCE}
        ${CMAKE_CURRENT_LIST_DIR}/heap-debug.c
    )
endif()

include(${CMAKE_CURRENT_LIST_DIR}/crypto/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/libc/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/textsearch/build.cmake)
