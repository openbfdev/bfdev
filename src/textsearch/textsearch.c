/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <textsearch.h>
#include <export.h>

static BFDEV_LIST_HEAD(algorithms);

static struct bfdev_ts_algorithm *
algorithm_find(const char *name)
{
    struct bfdev_ts_algorithm *algo;

    bfdev_list_for_each_entry(algo, &algorithms, list) {
        if (!strcmp(algo->name, name))
            return algo;
    }

    return NULL;
}

export struct bfdev_ts_context *
bfdev_textsearch_prepare(const struct bfdev_alloc *alloc, const char *name,
                         const void *pattern, size_t len, unsigned long flags)
{
    struct bfdev_ts_algorithm *algo;
    struct bfdev_ts_context *tsc;

    algo = algorithm_find(name);
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
bfdev_textsearch_register(struct bfdev_ts_algorithm *algo)
{
    if (!algo->name || !algo->find || !algo->prepare ||
        !algo->destroy || !algo->pattern_get || !algo->pattern_len)
        return -BFDEV_EINVAL;

    if (algorithm_find(algo->name))
        return -BFDEV_EALREADY;

    bfdev_list_add(&algorithms, &algo->list);
    return -BFDEV_ENOERR;
}

export void
bfdev_textsearch_unregister(struct bfdev_ts_algorithm *algo)
{
    if (algorithm_find(algo->name))
        return;

    bfdev_list_del(&algo->list);
}
