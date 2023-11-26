/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "cache-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <bfdev/cache.h>
#include <bfdev/log.h>

#define TEST_LOOP 100
#define TEST_SIZE 10
#define TEST_MASK 30

int main(int argc, const char *argv[])
{
    struct bfdev_cache_head *cache;
    unsigned int count;

    cache = bfdev_cache_create("lfu", NULL, TEST_SIZE, 1);
    if (!cache)
        return 1;

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        struct bfdev_cache_node *node;
        unsigned int value, verify;

        value = (unsigned int)rand() % TEST_MASK;
        node = bfdev_cache_get(cache, value);
        if (!node)
            return 1;

        if (node->status == BFDEV_CACHE_PENDING) {
            bfdev_log_info("test%u cache miss: %u\n", count, value);
            node->pdata = (void *)(uintptr_t)value;
            bfdev_cache_committed(cache);
        } else {
            verify = (uintptr_t)node->pdata;
            if (value == verify)
                bfdev_log_info("test%u cache hit: %u passed\n", count, verify);
            else {
                bfdev_log_err("test%u cache hit: %u failed\n", count, verify);
                return 1;
            }
        }

        bfdev_cache_put(cache, node);
    }

    bfdev_log_notice("cache changed = %lu\n", cache->changed);
    bfdev_log_notice("cache starve = %lu\n", cache->starve);
    bfdev_log_notice("cache hits = %lu\n", cache->hits);
    bfdev_log_notice("cache misses = %lu\n", cache->misses);
    bfdev_cache_destroy(cache);

    return 0;
}
