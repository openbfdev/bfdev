/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/knuth.h>
#include <bfdev/stdlib.h>
#include <export.h>

static __bfdev_noinline void
knuth_swap(bfdev_size_t cells, void *cel1, void *cel2)
{
    void *buff;

    /* alloca hates inline */
    buff = bfdev_alloca(cells);

    bfdev_memcpy(buff, cel1, cells);
    bfdev_memcpy(cel1, cel2, cells);
    bfdev_memcpy(cel2, buff, cells);
}

export int
bfdev_knuth(void *base, bfdev_size_t num, bfdev_size_t cells)
{
    bfdev_size_t idx1, idx2;

    if (bfdev_unlikely(!base || !num || !cells))
        return -BFDEV_EINVAL;

    for (idx1 = num - 1; idx1; --idx1) {
        idx2 = bfdev_rand() % (idx1 + 1);
        if (bfdev_unlikely(idx1 == idx2))
            continue;
        knuth_swap(cells, base + idx1 * cells, base + idx2 * cells);
    }

    return -BFDEV_ENOERR;
}
