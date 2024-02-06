# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

file(GLOB BFDEV_HEADER
    ${BFDEV_HEADER_PATH}/bfdev/*.h
)

file(GLOB BFDEV_ASM_HEADER
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic/*.h
)

file(GLOB BFDEV_ARCH_ASM_HEADER
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm/*.h
)

file(GLOB BFDEV_GENERATED_HEADER
    ${BFDEV_GENERATED_PATH}/*.h
)

include(${BFDEV_ARCH_PATH}/build.cmake)
include(${BFDEV_SOURCE_PATH}/build.cmake)
