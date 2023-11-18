/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BSEARCH_H_
#define _BFDEV_BSEARCH_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

static inline void *
bfdev_bsearch_inline(const void *base, size_t num, size_t esize,
                     bfdev_find_t find, void *pdata)
{
    const void *pivot;
    long result;

    while (num) {
        pivot = base + (num >> 1) * esize;
        result = find(pivot, pdata);

        if (!result)
            return (void *)pivot;

        if (result < 0) {
            base = pivot + esize;
            num--;
        }

        num >>= 1;
    }

    return NULL;
}

/**
 * bfdev_bsearch() - binary search an array of elements.
 * @base: pointer to first element to search.
 * @num: number of elements.
 * @esize: size of each element.
 * @cmp: pointer to comparison function.
 * @pdata: pointer to item being searched for.
 */
extern void *
bfdev_bsearch(const void *base, size_t num, size_t esize,
              bfdev_find_t find, void *pdata);

BFDEV_END_DECLS

#endif /* _BFDEV_BSEARCH_H_ */
