/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_KNUTH_H_
#define _BFDEV_KNUTH_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_knuth() - Knuth-Shuffle algorithm.
 * @base: pointer to data to sort.
 * @num: number of elements.
 * @cells: size of each element.
 *
 * The algorithm takes a list of all the elements of the sequence, and
 * continually determines the next element in the shuffled sequence
 * by randomly drawing an element from the list until no elements remain.
 * The algorithm produces an unbiased permutation, every permutation
 * is equally likely.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_knuth(void *base, bfdev_size_t num, bfdev_size_t cells);

BFDEV_END_DECLS

#endif /* _BFDEV_KNUTH_H_ */
