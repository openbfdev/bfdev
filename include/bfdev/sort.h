/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SORT_H_
#define _BFDEV_SORT_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_sort() - Heap sort an array of elements.
 * @base: pointer to data to sort.
 * @num: number of elements.
 * @cells: size of each element.
 * @cmp: pointer to comparison function.
 * @pdata: private data passed to comparison function.
 *
 * Sorting time is O(n log n) both on average and worst-case. While
 * quicksort is slightly faster on average, it suffers from exploitable
 * O(n^2) worst-case behavior and extra memory requirements.
 */
extern int
bfdev_sort(void *base, size_t num, size_t cells, bfdev_cmp_t cmp, void *pdata);

BFDEV_END_DECLS

#endif /* _BFDEV_SORT_H_ */
