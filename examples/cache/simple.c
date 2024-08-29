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

static unsigned long
cache_hash(const void *tag, void *pdata)
{
    return (unsigned long)(uintptr_t)tag;
}

static long
cache_find(const void *node, const void *tag, void *pdata)
{
    return node != tag;
}

static const bfdev_cache_ops_t
cache_ops = {
    .hash = cache_hash,
    .find = cache_find,
};

static int
cache_test(const char *name)
{
    bfdev_cache_head_t *cache;
    unsigned int count;

    cache = bfdev_cache_create(name, NULL, &cache_ops, TEST_SIZE, 1, NULL);
    if (!cache)
        return 1;

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_cache_node_t *node;
        unsigned int value, verify;

        value = (unsigned int)rand() % TEST_MASK;
        node = bfdev_cache_get(cache, (void *)(uintptr_t)value);
        if (!node)
            return 1;

        if (node->status == BFDEV_CACHE_PENDING) {
            bfdev_log_info("%s test%u cache miss: %u\n", name, count, value);
            node->data = (void *)(uintptr_t)value;
            bfdev_cache_committed(cache);
        } else {
            verify = (uintptr_t)node->data;
            if (value == verify)
                bfdev_log_info("%s test%u cache hit: %u passed\n",
                               name, count, verify);
            else {
                bfdev_log_err("%s test%u cache hit: %u failed\n",
                              name, count, verify);
                return 1;
            }
        }

        bfdev_cache_put(cache, node);
    }

    bfdev_log_notice("%s cache changed = %lu\n", name, cache->changed);
    bfdev_log_notice("%s cache starve = %lu\n", name, cache->starve);
    bfdev_log_notice("%s cache hits = %lu\n", name, cache->hits);
    bfdev_log_notice("%s cache misses = %lu\n", name, cache->misses);
    bfdev_cache_destroy(cache);

    return 0;
}

int
main(int argc, const char *argv[])
{
    int retval;

    retval = cache_test("lru");
    if (retval)
        return retval;

    retval = cache_test("lfu");
    if (retval)
        return retval;

    return 0;
}
