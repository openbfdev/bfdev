/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <bfdev/stringify.h>

#if !defined(GENCRC_NAME) || !defined(GENCRC_TYPE) || \
    !defined(GENCRC_BITS) || !defined(GENCRC_WIDE) || !defined(GENCRC_BELE)
# error "Compilation parameters not defined"
#endif

#define NAME_STRING __bfdev_stringify(GENCRC_NAME)
#define TYPE_STRING __bfdev_stringify(GENCRC_TYPE)
#define BITS_STRING __bfdev_stringify(GENCRC_BITS)
#define WIDE_STRING __bfdev_stringify(GENCRC_WIDE)

#define CRC_TABLE_BITS 8
#define CRC_TABLE_SIZE (1U << CRC_TABLE_BITS)
#define ENTRIES_PER_LINE 4

#if GENCRC_BELE
static void /* little endian */
table_generic(unsigned int rows, GENCRC_TYPE poly,
              GENCRC_TYPE (*table)[CRC_TABLE_SIZE])
{
    unsigned int count, index;
    GENCRC_TYPE crc;

    crc = 1;
    for (count = CRC_TABLE_SIZE >> 1; count; count >>= 1) {
        crc = (crc >> 1) ^ ((crc & 1) ? poly : 0);
        for (index = 0; index < CRC_TABLE_SIZE; index += 2 * count)
            table[0][count + index] = crc ^ table[0][index];
    }

    for (count = 0; count < CRC_TABLE_SIZE; ++count) {
        crc = table[0][count];
        for (index = 1; index < rows; ++index) {
            crc = table[0][crc & 0xff] ^ (crc >> 8);
            table[index][count] = crc;
        }
    }
}
#else
static void /* big endian */
table_generic(unsigned int rows, GENCRC_TYPE poly,
              GENCRC_TYPE (*table)[CRC_TABLE_SIZE])
{
    unsigned int count, index;
    GENCRC_TYPE crc;

    crc = 1ULL << (GENCRC_BITS - 1);
    for (count = 1; count < CRC_TABLE_SIZE; count <<= 1) {
        crc = (crc << 1) ^ ((crc & (1ULL << (GENCRC_BITS - 1))) ? poly : 0);
        for (index = 0; index < count; ++index)
            table[0][count + index] = crc ^ table[0][index];
    }

    for (count = 0; count < CRC_TABLE_SIZE; ++count) {
        crc = table[0][count];
        for (index = 1; index < rows; ++index) {
            crc = table[0][(crc >> (GENCRC_BITS - 8)) & 0xff] ^ (crc << 8);
            table[index][count] = crc;
        }
    }
}
#endif

static void
table_dump(unsigned int rows, const char *trans,
           GENCRC_TYPE (*table)[CRC_TABLE_SIZE])
{
    unsigned int count, index;

    for (index = 0 ; index < rows; ++index) {
        printf("\t{");
        for (count = 0; count < CRC_TABLE_SIZE; ++count) {
            if (count % ENTRIES_PER_LINE == 0)
                printf("\n\t\t");

            if (trans) {
                printf("%s((" TYPE_STRING ")0x%" WIDE_STRING "."
                       WIDE_STRING "llxULL), ", trans,
                       (unsigned long long)table[index][count]);
            } else {
                printf("(" TYPE_STRING ")0x%" WIDE_STRING
                       "." WIDE_STRING "llxULL, ",
                       (unsigned long long)table[index][count]);
            }
        }
        printf("\n\t},\n");
    }
}

int main(int argc, char *argv[])
{
    const char *name, *table, *trans;
    unsigned int rows;
    GENCRC_TYPE poly;
    void *buff;

    if (argc != 5 && argc != 6) {
        fprintf(stderr, "usage: %s name table rows poly [trans]\n", argv[0]);
        return -1;
    }

    name = argv[1];
    table = argv[2];
    rows = (unsigned int)strtoul(argv[3], NULL, 0);
    poly = (GENCRC_TYPE)strtoull(argv[4], NULL, 0);

    trans = NULL;
    if (argc == 6)
        trans = argv[5];

    if (!rows) {
        fprintf(stderr, "error: rows cannot be zero\n");
        return -1;
    }

    buff = calloc(1, sizeof(GENCRC_TYPE) * CRC_TABLE_SIZE * rows);
    if (!buff)
        return -1;

    printf(
        "/*\n"
        " * Automatically generated file; DO NOT EDIT.\n"
        " * bfdev scripts/gen-" NAME_STRING "\n"
        " */\n"
        "\n"
        "/*\n"
        " * This is the %s table.\n"
        " * Generated with:\n"
        " * width = " BITS_STRING " bits\n"
        " * poly = 0x%" WIDE_STRING "." WIDE_STRING "llx\n"
        " * byteorder = %s-endian\n"
        " */\n"
        "\n"
        "static const " TYPE_STRING " %s[%d][%d] = {\n",
        name, (unsigned long long)poly,
        GENCRC_BELE ? "little" : "big",
        table, rows, CRC_TABLE_SIZE
    );

    table_generic(rows, poly, buff);
    table_dump(rows, trans, buff);

    printf("};\n");
    free(buff);

    return 0;
}
