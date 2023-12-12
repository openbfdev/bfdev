/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "minpool-selftest"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/errno.h>
#include <bfdev/minpool.h>
#include <bfdev/bitmap.h>
#include <bfdev/size.h>

#define POOL_SIZE BFDEV_SZ_32MiB
#define TEST_SIZE BFDEV_SZ_16MiB
#define TEST_LOOP 100

static int
test_minpool(struct bfdev_minpool_head *pool)
{
    BFDEV_DEFINE_BITMAP(bitmap, TEST_LOOP);
    void *result[TEST_LOOP];
    unsigned int count, index;
    size_t size;

    for (count = 0; count < TEST_LOOP; ++count) {
        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result[count] = bfdev_minpool_alloc(pool, size);
        bfdev_log_info("minpool random alloc%02u: %p\n",
                       count, result[count]);
        if (!result[count])
            return 1;
        memset(result[count], 0, size);
    }

    bfdev_bitmap_zero(bitmap, TEST_LOOP);
    for (count = 0; count < TEST_LOOP;) {
        index = (unsigned int)rand() % TEST_LOOP;
        if (bfdev_bit_test(bitmap, index))
            continue;

        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result[index] = bfdev_minpool_realloc(pool, result[index], size);
        bfdev_log_info("minpool random realloc%02u: %p\n",
                       count, result[index]);
        if (!result[index])
            return 1;
        memset(result[index], 0, size);

        bfdev_bit_set(bitmap, index);
        count++;
    }

    bfdev_bitmap_zero(bitmap, TEST_LOOP);
    for (count = 0; count < TEST_LOOP;) {
        index = (unsigned int)rand() % TEST_LOOP;
        if (bfdev_bit_test(bitmap, index))
            continue;

        bfdev_log_info("minpool random free%02d: %p %d\n",
                       count, result[index], index);
        bfdev_minpool_free(pool, result[index]);

        bfdev_bit_set(bitmap, index);
        count++;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    struct bfdev_minpool_head pool;
    void *buffer;
    int retval;

    buffer = malloc(POOL_SIZE);
    if (!buffer)
        return 1;

    bfdev_log_info("Setup first-fit minpool...\n");
    bfdev_minpool_setup(&pool, bfdev_minpool_first_fit, buffer, POOL_SIZE);
    retval = test_minpool(&pool);
    if (retval && pool.avail != POOL_SIZE)
        return 1;

    bfdev_log_info("Setup best-fit minpool...\n");
    bfdev_minpool_setup(&pool, bfdev_minpool_best_fit, buffer, POOL_SIZE);
    retval = test_minpool(&pool);
    if (retval && pool.avail != POOL_SIZE)
        return 1;

    bfdev_log_info("Setup worst-fit minpool...\n");
    bfdev_minpool_setup(&pool, bfdev_minpool_worst_fit, buffer, POOL_SIZE);
    retval = test_minpool(&pool);
    if (retval && pool.avail != POOL_SIZE)
        return 1;

    free(buffer);

    return 0;
}
