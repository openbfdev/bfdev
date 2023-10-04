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

/**
 * bfdev_matrix_add() - adding two matrices.
 * @va: first addend matrix.
 * @vb: second addend matrix.
 */
extern struct bfdev_matrix *
bfdev_matrix_add(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

/**
 * bfdev_matrix_sub() - subtracting two matrices.
 * @va: minuend; matrix to subtract from.
 * @vb: subtrahend; matrix to subtract from @va.
 */
extern struct bfdev_matrix *
bfdev_matrix_sub(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

/**
 * bfdev_matrix_mul() - multiplying two matrices.
 * @va: first factor matrix.
 * @vb: second factor matrix.
 */
extern struct bfdev_matrix *
bfdev_matrix_mul(const struct bfdev_alloc *alloc, const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb);

/**
 * bfdev_matrix_copy() - copy a new matrix.
 * @var: variables to copy.
 */
extern struct bfdev_matrix *
bfdev_matrix_copy(const struct bfdev_alloc *alloc,
                  const struct bfdev_matrix *var);

/**
 * bfdev_matrix_create() - create a new matrix.
 * @row: number of rows in the matrix.
 * @col: number of columns in the matrix.
 */
extern struct bfdev_matrix *
bfdev_matrix_create(const struct bfdev_alloc *alloc,
                    unsigned int row, unsigned int col);

/**
 * bfdev_matrix_destory() - destory a matrix.
 * @var: variables to destory.
 */
extern void
bfdev_matrix_destory(const struct bfdev_alloc *alloc,
                     const struct bfdev_matrix *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MATRIX_H_ */
