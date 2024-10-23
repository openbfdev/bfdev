/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/tokenbucket.h>
#include <bfdev/time.h>
#include <export.h>

export bool
bfdev_tokenbucket(bfdev_tokenbucket_t *limit, bfdev_time_t current)
{
    unsigned int generate;

    if (!limit->interval)
        return true;

    if (bfdev_unlikely(!limit->last)) {
        limit->current = limit->capacity;
        limit->last = current;
    }

    /* generate token */
    generate = bfdev_time_sub(current, limit->last) / limit->interval;
    limit->last = bfdev_time_add(generate * limit->interval, limit->last);
    limit->current = bfdev_min(limit->current + generate, limit->capacity);

    if (bfdev_unlikely(!limit->current)) {
        limit->missed++;
        return false;
    }

    limit->current--;
    limit->passed++;

    return true;
}
