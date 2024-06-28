/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "crypto-bandwidth"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/sha1.h>
#include <bfdev/sha2.h>
#include <bfdev/md5.h>
#include <bfdev/log.h>
#include <bfdev/size.h>
#include "../time.h"

#define TEST_SIZE BFDEV_SZ_1MiB
#define TEST_LOOP 3

static void
sha1_test(void *buff)
{
    uint8_t digest[BFDEV_SHA1_DIGEST_SIZE];
    bfdev_sha1_ctx_t sha1;
    unsigned int count, loop;

    bfdev_sha1_init(&sha1);
    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_sha1_update(&sha1, buff, TEST_SIZE);
            bfdev_sha1_finish(&sha1, digest);
            0;
        );
        bfdev_log_info("sha1 bandwidth %u: %uMiB/s\n", count, loop);
    }
}

static void
sha224_test(void *buff)
{
    uint8_t digest[BFDEV_SHA224_DIGEST_SIZE];
    bfdev_sha2_ctx_t sha2;
    unsigned int count, loop;

    bfdev_sha224_init(&sha2);
    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_sha2_update(&sha2, buff, TEST_SIZE);
            bfdev_sha224_finish(&sha2, digest);
            0;
        );
        bfdev_log_info("sha224 bandwidth %u: %uMiB/s\n", count, loop);
    }
}

static void
sha256_test(void *buff)
{
    uint8_t digest[BFDEV_SHA256_DIGEST_SIZE];
    bfdev_sha2_ctx_t sha2;
    unsigned int count, loop;

    bfdev_sha256_init(&sha2);
    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_sha2_update(&sha2, buff, TEST_SIZE);
            bfdev_sha256_finish(&sha2, digest);
            0;
        );
        bfdev_log_info("sha256 bandwidth %u: %uMiB/s\n", count, loop);
    }
}

static void
md5_test(void *buff)
{
    uint8_t digest[BFDEV_MD5_DIGEST_SIZE];
    bfdev_md5_ctx_t md5;
    unsigned int count, loop;

    bfdev_md5_init(&md5);
    for (count = 0; count < TEST_LOOP; ++count) {
        EXAMPLE_TIME_LOOP(&loop, 1000,
            bfdev_md5_update(&md5, buff, TEST_SIZE);
            bfdev_md5_finish(&md5, digest);
            0;
        );
        bfdev_log_info("md5 bandwidth %u: %uMiB/s\n", count, loop);
    }
}

int
main(int argc, char const *argv[])
{
    uint8_t *buff;
    size_t index;

    buff = malloc(TEST_SIZE);
    if (!buff)
        return 1;

    srand(time(NULL));
    for (index = 0; index < TEST_SIZE; ++index)
        buff[index] = (uint8_t)rand();

    sha1_test(buff);
    sha224_test(buff);
    sha256_test(buff);
    md5_test(buff);

    free(buff);
    return 0;
}
