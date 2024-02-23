/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "arc4-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/arc4.h>
#include <bfdev/crc.h>
#include <bfdev/log.h>
#include <bfdev/size.h>
#include "../time.h"

#define TEST_SIZE BFDEV_SZ_1MiB
#define TEST_LOOP 3
#define TEST_KEY 64

int main(int argc, const char *argv[])
{
    bfdev_arc4_ctx_t sctx, dctx;
    unsigned int count, loop;
    uint32_t cksum, check;
    uint8_t *dbuff, *sbuff;

    sbuff = malloc(TEST_SIZE);
    if (!sbuff)
        return 1;

    dbuff = malloc(TEST_SIZE);
    if (!dbuff)
        return 1;

    srand(time(NULL));
    for (count = 0; count < 64; ++count)
        sbuff[count] = (uint8_t)rand();
    bfdev_arc4_setkey(&sctx, sbuff, TEST_KEY);

    for (count = 0; count < TEST_SIZE; ++count)
        sbuff[count] = (uint8_t)rand();

    cksum = bfdev_crc32(sbuff, TEST_SIZE, (uint32_t)~0UL);
    bfdev_log_info("start checksum: %#010x\n", cksum);

    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            dctx = sctx;
            bfdev_arc4_trans(&sctx, dbuff, sbuff, TEST_SIZE);
            bfdev_arc4_trans(&dctx, sbuff, dbuff, TEST_SIZE);
            0;
        );
        bfdev_log_info("bandwidth %u: %uMiB/s\n", count, loop * 2);
    }

    check = bfdev_crc32(sbuff, TEST_SIZE, (uint32_t)~0UL);
    if (cksum != check) {
        bfdev_log_err("verification failed\n");
        return 1;
    }
    bfdev_log_info("verification pass\n");

    free(sbuff);
    free(dbuff);

    return 0;
}
