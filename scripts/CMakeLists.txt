# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set(HOST_C_FLAGS
    ${HOST_C_FLAGS}
    -Wall
    -Wextra
    -I ${PROJECT_SOURCE_DIR}/include
    -I ${PROJECT_BINARY_DIR}/generated
)

host_target(gen-crc8 gen-crc8.c)
host_target(gen-crc16 gen-crc16.c)
host_target(gen-crc16be gen-crc16be.c)
host_target(gen-crc32 gen-crc32.c)
host_target(gen-crc32be gen-crc32be.c)
host_target(gen-crc64 gen-crc64.c)
host_target(gen-crc64be gen-crc64be.c)

macro(generate_crctbl proc path table rows poly trans)
    add_custom_command(
        TARGET ${proc}
        POST_BUILD COMMAND
        bash -c \"
            mkdir -p ${BFDEV_GENERATED_PATH}/bfdev/crypto &&
            ${PROJECT_BINARY_DIR}/scripts/${proc}
                ${table} ${rows} ${poly} ${trans} >
                ${BFDEV_GENERATED_PATH}/bfdev/crypto/${path}
        \"
    )
endmacro()

if(BFDEV_CRC_EXTEND)
    set(CRC_ROWS 8)
else()
    set(CRC_ROWS 1)
endif()

generate_crctbl(
    gen-crc8 crc7-table.h
    "CRC-7"
    bfdev_crc7_table ${CRC_ROWS}
    0x12 ""
)

generate_crctbl(
    gen-crc8 crc8-table.h
    "CRC-8"
    bfdev_crc8_table ${CRC_ROWS}
    0x07 ""
)

generate_crctbl(
    gen-crc16 crc16-table.h
    "CRC-16"
    bfdev_crc16_table ${CRC_ROWS}
    0xa001 "bfdev_cpu_to_le16"
)

generate_crctbl(
    gen-crc16 crc-ccitt-table.h
    "CRC-CCITT"
    bfdev_crc_ccitt_table ${CRC_ROWS}
    0x8408 "bfdev_cpu_to_le16"
)

generate_crctbl(
    gen-crc16be crc-itut-table.h
    "CRC-ITUT"
    bfdev_crc_itut_table ${CRC_ROWS}
    0x1021 "bfdev_cpu_to_be16"
)

generate_crctbl(
    gen-crc16be crc-t10dif-table.h
    "CRC-T10DIF"
    bfdev_crc_t10dif_table ${CRC_ROWS}
    0x8bb7 "bfdev_cpu_to_be16"
)

generate_crctbl(
    gen-crc32 crc32-table.h
    "CRC-32"
    bfdev_crc32_table ${CRC_ROWS}
    0xedb88320 "bfdev_cpu_to_le32"
)

generate_crctbl(
    gen-crc64be crc64-table.h
    "CRC-64"
    bfdev_crc64_table ${CRC_ROWS}
    0x42f0e1eba9ea3693 "bfdev_cpu_to_be64"
)

generate_crctbl(
    gen-crc64 crc-rocksoft-table.h
    "CRC-ROCKSOFT"
    bfdev_crc_rocksoft_table ${CRC_ROWS}
    0x9a6c9329ac4bc9b5 "bfdev_cpu_to_le64"
)
