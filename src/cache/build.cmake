# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/cache.c
    ${CMAKE_CURRENT_LIST_DIR}/lru.c
    ${CMAKE_CURRENT_LIST_DIR}/lfu.c
)
