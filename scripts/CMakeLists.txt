# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(gen-crc32 gen-crc32.c)
add_custom_command(
    TARGET gen-crc32
    POST_BUILD COMMAND
    mkdir -p ${BFDEV_GENERATED_PATH}/bfdev/crypto
    POST_BUILD COMMAND
    ${PROJECT_BINARY_DIR}/scripts/gen-crc32
    bfdev_crc32_table 8 0xedb88320 bfdev_cpu_to_le32_const >
    ${BFDEV_GENERATED_PATH}/bfdev/crypto/crc32-table.h
)
