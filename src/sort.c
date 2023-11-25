/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/sort.h>
#include <export.h>

static __bfdev_always_inline void
sort_swap(size_t cells, void *cela, void *celb)
{
    uint8_t *buff;

    buff = bfdev_alloca(cells);

    memcpy(buff, cela, cells);
    memcpy(cela, celb, cells);
    memcpy(celb, buff, cells);
}

static __bfdev_attribute_const __bfdev_always_inline size_t
parent(size_t cells, unsigned int lsbit, size_t index)
{
    index -= cells;
    index -= cells & -(index & lsbit);
    return index >> 1;
}

export int
bfdev_qsort(void *base, size_t num, size_t cells, bfdev_cmp_t cmp, void *pdata)
{
    const unsigned int lsbit = cells & -cells;
    size_t size = num * cells;
    size_t ida, idb, idc, idd;

    ida = (num / 2) * cells;
    if (!base || !cmp || !ida)
        return -BFDEV_EINVAL;

    for (;;) {
        if (ida)
            ida -= cells;
        else if (size -= cells)
            sort_swap(cells, base, base + size);
        else
            break;

        for (idb = ida; idc = 2 * idb + cells, (idd = idc + cells) < size;)
            idb = cmp(base + idc, base + idd, pdata) >= 0 ? idc : idd;
        if (idd == size)
            idb = idc;

        while (ida != idb && cmp(base + ida, base + idb, pdata) >= 0)
            idb = parent(cells, lsbit, idb);

        for (idc = idb; ida != idb;) {
            idb = parent(cells, lsbit, idb);
            sort_swap(cells, base + idb, base + idc);
        }
    }

    return -BFDEV_ENOERR;
}
