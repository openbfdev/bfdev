/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "sha1-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/sha1.h>
#include <bfdev/log.h>
#include <bfdev/size.h>
#include "../time.h"

#define TEST_SIZE BFDEV_SZ_1MiB
#define TEST_LOOP 3

int
main(int argc, char const *argv[])
{
    uint8_t digest[BFDEV_SHA1_DIGEST_SIZE];
    unsigned int count, loop;
    bfdev_sha1_ctx_t sha1;
    uint8_t *buff;
    size_t index;

    buff = malloc(TEST_SIZE);
    if (!buff)
        return 1;

    srand(time(NULL));
    for (index = 0; index < TEST_SIZE; ++index)
        buff[index] = (uint8_t)rand();

    bfdev_sha1_init(&sha1);
    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_sha1_update(&sha1, buff, TEST_SIZE);
            bfdev_sha1_finish(&sha1, digest);
            0;
        );
        bfdev_log_info("bandwidth %u: %uMiB/s\n", count, loop);
    }

    free(buff);
    return 0;
}
