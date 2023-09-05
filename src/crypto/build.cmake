# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set(BFDEV_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/base32.c
    ${CMAKE_CURRENT_LIST_DIR}/base64.c
    ${CMAKE_CURRENT_LIST_DIR}/crc4.c
    ${CMAKE_CURRENT_LIST_DIR}/crc7.c
    ${CMAKE_CURRENT_LIST_DIR}/crc8.c
    ${CMAKE_CURRENT_LIST_DIR}/crc16.c
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/crc64.c
    ${CMAKE_CURRENT_LIST_DIR}/crc-ccitt.c
    ${CMAKE_CURRENT_LIST_DIR}/crc-itut.c
    ${CMAKE_CURRENT_LIST_DIR}/crc-rocksoft.c
    ${CMAKE_CURRENT_LIST_DIR}/crc-t10dif.c
)
