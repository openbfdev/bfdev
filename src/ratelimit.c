/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/ratelimit.h>
#include <bfdev/time.h>
#include <export.h>

export bfdev_bool
bfdev_ratelimit(bfdev_ratelimit_t *limit, bfdev_time_t current)
{
    bfdev_bool accept;

    if (!limit->interval)
        return bfdev_true;

    if (bfdev_unlikely(!limit->begin))
        limit->begin = current;

    if (bfdev_time_after(current, bfdev_time_add(limit->begin, limit->interval))) {
        limit->begin = current;
        limit->passed = 0;
    }

    if (bfdev_likely(limit->burst > limit->passed)) {
        limit->passed++;
        accept = bfdev_true;
    } else {
        limit->missed++;
        accept = bfdev_false;
    }

    return accept;
}
