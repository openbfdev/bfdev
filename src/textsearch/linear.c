/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/textsearch.h>
#include <export.h>

static unsigned int
linear_next(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss,
            unsigned int consumed, const void **dest)
{
    bfdev_ts_linear_t *linear;

    linear = tss->pdata;
    if (bfdev_likely(consumed < linear->len)) {
        *dest = linear->data + consumed;
        return linear->len - consumed;
    }

    return 0;
}

export unsigned int
bfdev_textsearch_linear(bfdev_ts_context_t *tsc, bfdev_ts_linear_t *linear,
                        const void *data, unsigned int len)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;
    tsc->next_block = linear_next;

    linear->data = data;
    linear->len = len;
    linear->tss.offset = 0;
    linear->tss.pdata = linear;

    return algo->find(tsc, &linear->tss);
}
