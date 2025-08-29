# SPDX-License-Identifier: LGPL-3.0-or-later
#
# Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/cache.c
    ${CMAKE_CURRENT_LIST_DIR}/lru.c
    ${CMAKE_CURRENT_LIST_DIR}/lfu.c
)
