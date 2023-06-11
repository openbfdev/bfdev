# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(gen-crc16 gen-crc16.c)
add_executable(gen-crc32 gen-crc32.c)
add_executable(gen-crc64 gen-crc64.c)

macro(generate_crctbl proc path table rows poly trans)
    add_custom_command(
        TARGET ${proc}
        POST_BUILD COMMAND
        mkdir -p ${BFDEV_GENERATED_PATH}/bfdev/crypto
        POST_BUILD COMMAND
        ${PROJECT_BINARY_DIR}/scripts/${proc}
        ${table} ${rows} ${poly} ${trans} >
        ${BFDEV_GENERATED_PATH}/bfdev/crypto/${path}
    )
endmacro()

generate_crctbl(
    gen-crc16 crc16-table.h
    bfdev_crc16_table 8 0xa001 ""
)

generate_crctbl(
    gen-crc16 crc-ccitt-table.h
    bfdev_crc_ccitt_table 8 0x8408 ""
)

generate_crctbl(
    gen-crc32 crc32-table.h
    bfdev_crc32_table 8 0xedb88320 ""
)

generate_crctbl(
    gen-crc64 crc-rocksoft-table.h
    bfdev_crc_rocksoft_table 8 0x9a6c9329ac4bc9b5 ""
)
