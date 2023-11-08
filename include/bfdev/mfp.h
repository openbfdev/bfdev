/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MFP_H_
#define _BFDEV_MFP_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stdbool.h>
#include <bfdev/bits.h>
#include <bfdev/array.h>
#include <bfdev/limits.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

#define BFDEV_MFP_TYPE unsigned long
#define BFDEV_MFP_WIDE (BFDEV_BITS_PER_LONG / 2)
#define BFDEV_MFP_BASE (BFDEV_BIT(BFDEV_MFP_WIDE) - 1)

struct bfdev_mfp {
    unsigned int length;
    unsigned int scale;
    bool plus;

    BFDEV_MFP_TYPE *value;
    BFDEV_MFP_TYPE block[];
};

extern const struct bfdev_mfp
bfdev_mfp_zero;

extern const struct bfdev_mfp
bfdev_mfp_one;

extern const struct bfdev_mfp *
bfdev_mfp_add(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
              const struct bfdev_mfp *vb);

extern const struct bfdev_mfp *
bfdev_mfp_sub(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
              const struct bfdev_mfp *vb);

extern struct bfdev_mfp *
bfdev_mfp_create(const struct bfdev_alloc *alloc, unsigned int length,
                 unsigned int scale);

extern void
bfdev_mfp_destory(const struct bfdev_alloc *alloc, const struct bfdev_mfp *val);

BFDEV_END_DECLS

#endif /* _BFDEV_MFP_H_ */
