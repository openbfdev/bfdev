/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MATRIX_H_
#define _BFDEV_MATRIX_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

struct bfdev_matrix {
    unsigned int row;
    unsigned int col;
    long values[];
};

extern struct bfdev_matrix *
bfdev_matrix_add(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

extern struct bfdev_matrix *
bfdev_matrix_sub(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

extern struct bfdev_matrix *
bfdev_matrix_mul(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

extern struct bfdev_matrix *
bfdev_matrix_create(const struct bfdev_alloc *alloc, unsigned int row,
                    unsigned int col);

extern void
bfdev_matrix_destory(const struct bfdev_alloc *alloc, struct bfdev_matrix *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MATRIX_H_ */
