/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define CRC_TABLE_BITS 8
#define CRC_TABLE_SIZE (1U << CRC_TABLE_BITS)
#define ENTRIES_PER_LINE 4

static void
crc32_generic(unsigned int rows, uint32_t poly,
              uint32_t (*table)[CRC_TABLE_SIZE])
{
    unsigned int count, index;
    uint32_t crc = 1;

    table[0][0] = 0;
    for (count = CRC_TABLE_SIZE >> 1; count; count >>= 1) {
        crc = (crc >> 1) ^ ((crc & 1) ? poly : 0);
        for (index = 0; index < CRC_TABLE_SIZE; index += 2 * count)
            table[0][count + index] = crc ^ table[0][index];
    }

    for (count = 0; count < CRC_TABLE_SIZE; count++) {
        crc = table[0][count];
        for (index = 1; index < rows; index++) {
            crc = table[0][crc & 0xff] ^ (crc >> 8);
            table[index][count] = crc;
        }
    }
}

static void
table_dump(unsigned int rows, const char *trans,
           uint32_t (*table)[CRC_TABLE_SIZE])
{
    unsigned int count, index;

    for (index = 0 ; index < rows; ++index) {
        printf("\t{");
        for (count = 0; count < CRC_TABLE_SIZE; ++count) {
            if (count % ENTRIES_PER_LINE == 0)
                printf("\n\t\t");
            printf("%s(0x%8.8xUL), ", trans, table[index][count]);
        }
        printf("\n\t},\n");
    }
}

int main(int argc, char *argv[])
{
    uint32_t table[8][256];
    const char *trans = "";
    unsigned int rows;
    uint32_t poly;

    if (argc < 4) {
        printf("gen-crc32: name rows poly trans\n");
        return -1;
    }

    if (argc > 4)
        trans = argv[4];

    rows = (unsigned int)strtoul(argv[2], NULL, 0);
    poly = (uint32_t)strtoul(argv[3], NULL, 0);

    printf("/*\n");
    printf(" * Automatically generated file; DO NOT EDIT.\n");
    printf(" * bfdev scipts/gen-crc32\n");
    printf(" */\n\n");

    printf("static const uint32_t %s[%d][%d] = {\n",
            argv[1], rows, CRC_TABLE_SIZE);
    crc32_generic(rows, poly, table);
    table_dump(rows, trans, table);
    printf("};\n");

    return 0;
}
