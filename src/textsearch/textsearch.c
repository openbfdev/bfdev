/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/textsearch.h>
#include <export.h>

static
BFDEV_LIST_HEAD(textsearch_algorithms);

static bfdev_ts_algorithm_t *
textsearch_algorithm_find(const char *name)
{
    bfdev_ts_algorithm_t *walk;

    bfdev_list_for_each_entry(walk, &textsearch_algorithms, list) {
        if (!bfport_strcmp(walk->name, name))
            return walk;
    }

    return NULL;
}

static bool
textsearch_algorithm_exist(bfdev_ts_algorithm_t *algo)
{
    bfdev_ts_algorithm_t *walk;

    bfdev_list_for_each_entry(walk, &textsearch_algorithms, list) {
        if (walk == algo)
            return true;
    }

    return false;
}

export bfdev_ts_context_t *
bfdev_textsearch_create(const bfdev_alloc_t *alloc, const char *name,
                        const void *pattern, size_t len, unsigned long flags)
{
    bfdev_ts_algorithm_t *algo;
    bfdev_ts_context_t *tsc;

    algo = textsearch_algorithm_find(name);
    if (!algo)
        return NULL;

    tsc = algo->prepare(alloc, pattern, len, flags);
    if (!tsc)
        return NULL;

    tsc->algo = algo;
    tsc->alloc = alloc;

    return tsc;
}

export int
bfdev_textsearch_register(bfdev_ts_algorithm_t *algo)
{
    if (!(algo->name && algo->find && algo->prepare &&
          algo->destroy && algo->pattern_get &&
          algo->pattern_len))
        return -BFDEV_EINVAL;

    if (textsearch_algorithm_find(algo->name))
        return -BFDEV_EALREADY;

    bfdev_list_add(&textsearch_algorithms, &algo->list);

    return -BFDEV_ENOERR;
}

export int
bfdev_textsearch_unregister(bfdev_ts_algorithm_t *algo)
{
    if (!textsearch_algorithm_exist(algo))
        return -BFDEV_ENOENT;

    bfdev_list_del(&algo->list);

    return -BFDEV_ENOERR;
}
