/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "memalloc-fuzzy"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bfdev/log.h>
#include <bfdev/errno.h>
#include <bfdev/memalloc.h>
#include <bfdev/knuth.h>
#include <bfdev/size.h>
#include <testsuite.h>

#define POOL_SIZE BFDEV_SZ_32MiB
#define TEST_SIZE BFDEV_SZ_16MiB
#define TEST_LOOP 4096

static int
test_memalloc(bfdev_memalloc_head_t *pool)
{
    bfdev_memalloc_chunk_t *node;
    void *result[TEST_LOOP];
    unsigned int count;
    size_t size;
    int retval;

    retval = -BFDEV_ENOERR;
    srand(time(NULL));

    for (count = 0; count < TEST_LOOP; ++count) {
        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result[count] = bfdev_memalloc_alloc(pool, size);
        if (!result[count])
            return -BFDEV_ENOMEM;
        memset(result[count], 0, size);
    }

    bfdev_knuth(result, TEST_LOOP, sizeof(*result));
    for (count = 0; count < TEST_LOOP; ++count) {
        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result[count] = bfdev_memalloc_realloc(pool, result[count], size);
        if (!result[count])
            return -BFDEV_ENOMEM;
        memset(result[count], 0, size);
    }

    bfdev_knuth(result, TEST_LOOP, sizeof(*result));
    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_memalloc_free(pool, result[count]);

    node = bfdev_list_first_entry(&pool->block_list, bfdev_memalloc_chunk_t, block);
    if (node->usize != POOL_SIZE - sizeof(bfdev_memalloc_chunk_t)) {
        bfdev_log_err("free node size leak %#lx -> %#lx\n", (unsigned long)POOL_SIZE -
                      sizeof(bfdev_memalloc_chunk_t), (unsigned long)node->usize);
        return -BFDEV_EFAULT;
    }

    if (pool->avail != POOL_SIZE - sizeof(bfdev_memalloc_chunk_t)) {
        bfdev_log_err("total available leak %#lx -> %#lx\n", (unsigned long)POOL_SIZE -
                      sizeof(bfdev_memalloc_chunk_t), (unsigned long)pool->avail);
        return -BFDEV_EFAULT;
    }

    return retval;
}

#define TEST_PREPARE(NAME, FUNC)                        \
static void *                                           \
NAME(int argc, const char *argv[])                      \
{                                                       \
    bfdev_memalloc_head_t *pool;                        \
    void *memory;                                       \
                                                        \
    pool = malloc(sizeof(*pool) + POOL_SIZE);           \
    if (!pool)                                          \
        return BFDEV_ERR_PTR(-BFDEV_ENOMEM);            \
                                                        \
    memory = (void *)pool + sizeof(*pool);              \
    bfdev_memalloc_init(pool, FUNC, memory, POOL_SIZE); \
                                                        \
    return pool;                                        \
}

static void
test_release(void *data)
{
    free(data);
}

TEST_PREPARE(first_fit_prepare, bfdev_memalloc_first_fit)
TEST_PREPARE(best_fit_prepare, bfdev_memalloc_best_fit)
TEST_PREPARE(worst_fit_prepare, bfdev_memalloc_worst_fit)

TESTSUITE(
    "memalloc:first-fit",
    first_fit_prepare, test_release,
    "memalloc first-fit fuzzy test"
) {
    return test_memalloc(data);
}

TESTSUITE(
    "memalloc:best-fit",
    best_fit_prepare, test_release,
    "memalloc best-fit fuzzy test"
) {
    return test_memalloc(data);
}

TESTSUITE(
    "memalloc:worst-fit",
    worst_fit_prepare, test_release,
    "memalloc worst-fit fuzzy test"
) {
    return test_memalloc(data);
}
