/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/textsearch.h>
#include <export.h>

static unsigned int
linear_next(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss,
            unsigned int consumed, const void **dest)
{
    struct bfdev_ts_linear *linear = tss->pdata;

	if (likely(consumed < linear->len)) {
		*dest = linear->data + consumed;
		return linear->len - consumed;
	}

    return 0;
}

export unsigned int
bfdev_textsearch_linear_find(struct bfdev_ts_context *tsc,
                             struct bfdev_ts_linear *linear,
                             const void *data, unsigned int len)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;

    linear->data = data;
    linear->len = len;
    linear->tss.offset = 0;
    linear->tss.pdata = linear;
    tsc->next_block = linear_next;

    return algo->find(tsc, &linear->tss);
}

export unsigned int
bfdev_textsearch_linear_next(struct bfdev_ts_context *tsc,
                             struct bfdev_ts_linear *linear)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    return algo->find(tsc, &linear->tss);
}
