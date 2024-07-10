/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/sort.h>
#include <export.h>

static __bfdev_noinline void
sort_swap(size_t cells, void *cel1, void *cel2)
{
    void *buff;

    /* alloca hates inline */
    buff = bfdev_alloca(cells);

    bfport_memcpy(buff, cel1, cells);
    bfport_memcpy(cel1, cel2, cells);
    bfport_memcpy(cel2, buff, cells);
}

static __bfdev_attribute_const __bfdev_always_inline size_t
parent(size_t cells, unsigned int lsbit, size_t index)
{
    index -= cells;
    index -= cells & -(index & lsbit);
    return index >> 1;
}

export int
bfdev_sort(void *base, size_t num, size_t cells, bfdev_cmp_t cmp, void *pdata)
{
    size_t idx1, idx2, idx3, idx4;
    size_t size, lsbit;

    idx1 = (num >> 1) * cells;
    if (bfdev_unlikely(!base || !cmp || !idx1))
        return -BFDEV_EINVAL;

    size = num * cells;
    lsbit = cells & -cells;

    for (;;) {
        if (idx1)
            idx1 -= cells;
        else if (size -= cells)
            sort_swap(cells, base, base + size);
        else
            break;

        idx2 = idx1;
        while (idx3 = 2 * idx2 + cells, (idx4 = idx3 + cells) < size)
            idx2 = cmp(base + idx3, base + idx4, pdata) >= 0 ? idx3 : idx4;

        if (idx4 == size)
            idx2 = idx3;

        while (idx1 != idx2 && cmp(base + idx1, base + idx2, pdata) >= 0)
            idx2 = parent(cells, lsbit, idx2);

        idx3 = idx2;
        while (idx1 != idx2) {
            idx2 = parent(cells, lsbit, idx2);
            sort_swap(cells, base + idx2, base + idx3);
        }
    }

    return -BFDEV_ENOERR;
}
