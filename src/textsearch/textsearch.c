/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/textsearch.h>
#include <export.h>

static BFDEV_LIST_HEAD(textsearch_algorithms);

static bfdev_ts_algorithm_t *
textsearch_algorithm_find(const char *name)
{
    bfdev_ts_algorithm_t *algo;

    bfdev_list_for_each_entry(algo, &textsearch_algorithms, list) {
        if (!strcmp(algo->name, name))
            return algo;
    }

    return NULL;
}

export bfdev_ts_context_t *
bfdev_textsearch_create(const struct bfdev_alloc *alloc, const char *name,
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

export void
bfdev_textsearch_unregister(bfdev_ts_algorithm_t *algo)
{
    if (textsearch_algorithm_find(algo->name))
        return;

    bfdev_list_del(&algo->list);
}
