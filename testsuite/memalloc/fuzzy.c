/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
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
#include <bfdev/size.h>
#include <testsuite.h>
#include <randpool.h>

#define POOL_SIZE BFDEV_SZ_32MiB
#define TEST_SIZE BFDEV_SZ_16MiB
#define TEST_LOOP 4096

static int
test_memalloc(bfdev_memalloc_head_t *pool)
{
    bfdev_memalloc_chunk_t *node;
    void *result, *data;
    unsigned int count;
    size_t size;
    int retval;

    DEFINE_RANDPOOL(rpool1);
    DEFINE_RANDPOOL(rpool2);
    retval = -BFDEV_ENOERR;

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result = bfdev_memalloc_alloc(pool, size);
        if (!result) {
            retval = BFDEV_ENOMEM;
            goto failed;
        }

        retval = randpool_put(&rpool1, result);
        if (retval)
            goto failed;

        memset(result, 0, size);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        data = randpool_get(&rpool1);
        size = (unsigned int)rand() % (TEST_SIZE / TEST_LOOP);
        result = bfdev_memalloc_realloc(pool, data, size);
        if (!result) {
            retval = BFDEV_ENOMEM;
            goto failed;
        }

        retval = randpool_put(&rpool2, result);
        if (retval)
            goto failed;

        memset(result, 0, size);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        data = randpool_get(&rpool2);
        bfdev_memalloc_free(pool, data);
    }

    node = bfdev_list_first_entry(&pool->block_list, bfdev_memalloc_chunk_t, block);
    if (node->usize != POOL_SIZE - sizeof(bfdev_memalloc_chunk_t)) {
        bfdev_log_err("free node size leak %#lx -> %#lx\n", POOL_SIZE -
                      sizeof(bfdev_memalloc_chunk_t), node->usize);
        retval = -BFDEV_EFAULT;
        goto failed;
    }

    if (pool->avail != POOL_SIZE - sizeof(bfdev_memalloc_chunk_t)) {
        bfdev_log_err("total available leak %#lx -> %#lx\n", POOL_SIZE -
                      sizeof(bfdev_memalloc_chunk_t), pool->avail);
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    randpool_release(&rpool1, NULL, NULL);
    randpool_release(&rpool2, NULL, NULL);
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
