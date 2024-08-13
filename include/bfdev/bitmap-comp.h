/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITMAP_COMP_H_
#define _BFDEV_BITMAP_COMP_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

extern bool
bfdev_bitmap_comp_equal(const unsigned long *src1, const unsigned long *src2,
                        unsigned int bits);

extern bool
bfdev_bitmap_comp_or_equal(const unsigned long *src1, const unsigned long *src2,
                           const unsigned long *src3, unsigned int bits);

extern bool
bfdev_bitmap_comp_intersects(const unsigned long *src1, const unsigned long *src2,
                             unsigned int bits);

extern bool
bfdev_bitmap_comp_and(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits);

extern bool
bfdev_bitmap_comp_andnot(unsigned long *dest, const unsigned long *src1,
                         const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_or(unsigned long *dest, const unsigned long *src1,
                     const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_xor(unsigned long *dest, const unsigned long *src1,
                      const unsigned long *src2, unsigned int bits);

extern void
bfdev_bitmap_comp_shl(unsigned long *dest, const unsigned long *src,
                      unsigned int shift, unsigned int bits);

extern void
bfdev_bitmap_comp_shr(unsigned long *dest, const unsigned long *src,
                      unsigned int shift, unsigned int bits);

extern void
bfdev_bitmap_comp_complement(unsigned long *dest, const unsigned long *src,
                             unsigned int bits);

extern void
bfdev_bitmap_comp_replace(unsigned long *bitmap, const unsigned long *oldp,
                          const unsigned long *newp, const unsigned long *mask,
                          unsigned int bits);

extern void
bfdev_bitmap_comp_set(unsigned long *bitmap, unsigned int start,
                      unsigned int bits);

extern void
bfdev_bitmap_comp_clr(unsigned long *bitmap, unsigned int start,
                      unsigned int bits);

/**
 * bfdev_bitmap_alloc - alloc a bitmap.
 * @bits: number of bits in the bitmap.
 * @flags: allocate flags.
 */
extern unsigned long *
bfdev_bitmap_alloc(const bfdev_alloc_t *alloc, unsigned int bits);

/**
 * bfdev_bitmap_zalloc - alloc and zeroed a bitmap.
 * @bits: number of bits in the bitmap.
 * @flags: allocate flags.
 */
extern unsigned long *
bfdev_bitmap_zalloc(const bfdev_alloc_t *alloc, unsigned int bits);

/**
 * bfdev_bitmap_free - free a bitmap.
 * @bitmap: bitmap to free.
 */
extern void
bfdev_bitmap_free(const bfdev_alloc_t *alloc, const unsigned long *bitmap);

BFDEV_END_DECLS

#endif /* _BFDEV_BITMAP_COMP_H_ */
