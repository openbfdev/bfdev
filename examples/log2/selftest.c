/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <bfdev/log2.h>

int main(int argc, const char *argv[])
{
    unsigned int count, result;

    for (count = 0; count < 32; ++count) {
        uint32_t value;

        value = 1UL << count;
        result = bfdev_ilog2(value);

        printf("log2 'bfdev_ilog2' u32 test: %u -> %u ", count, result);
        if (result != count) {
            printf("failed\n");
            return 1;
        }
        printf("passed\n");
    }

    for (count = 0; count < 64; ++count) {
        uint64_t value;

        value = 1ULL << count;
        result = bfdev_ilog2(value);

        printf("log2 'bfdev_ilog2' u64 test: %u -> %u ", count, result);
        if (result != count) {
            printf("failed\n");
            return 1;
        }
        printf("passed\n");
    }

    return 0;
}
