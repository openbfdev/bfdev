/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "base64-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/base64.h>
#include <bfdev/crc.h>
#include <bfdev/log.h>
#include <bfdev/size.h>
#include "../time.h"

#define TEST_SIZE BFDEV_SZ_1MiB
#define TEST_LOOP 3

int main(int argc, char const *argv[])
{
    unsigned int count, loop;
    uint8_t *dbuff, *sbuff;
    size_t dlen, slen, index;
    uint32_t cksum, check;

    slen = bfdev_base64_encode_length(TEST_SIZE);
    sbuff = malloc(slen);
    if (!sbuff)
        return 1;

    dlen = bfdev_base64_decode_length(slen);
    dbuff = malloc(dlen);
    if (!dbuff)
        return 1;

    srand(time(NULL));
    for (index = 0; index < dlen; ++index)
        dbuff[index] = (uint8_t)rand();

    cksum = bfdev_crc32(dbuff, dlen, (uint32_t)~0UL);
    bfdev_log_info("start checksum: %#010x\n", cksum);

    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_base64_encode(sbuff, dbuff, dlen);
            0;
        );
        bfdev_log_info("encode bandwidth %u: %uMiB/s\n", count, loop);

        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_base64_decode(dbuff, sbuff, slen);
            0;
        );
        bfdev_log_info("decode bandwidth %u: %uMiB/s\n", count, loop);
    }

    check = bfdev_crc32(dbuff, dlen, (uint32_t)~0UL);
    if (cksum != check) {
        bfdev_log_err("verification failed\n");
        return 1;
    }
    bfdev_log_info("verification pass\n");

    free(sbuff);
    free(dbuff);

    return 0;
}
