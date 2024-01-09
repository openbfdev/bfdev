/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MATRIX_H_
#define _BFDEV_MATRIX_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_matrix bfdev_matrix_t;

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
extern bfdev_matrix_t *
bfdev_matrix_add(const bfdev_alloc_t *alloc, const bfdev_matrix_t *va,
                 const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_sub() - subtracting two matrices.
 * @va: minuend; matrix to subtract from.
 * @vb: subtrahend; matrix to subtract from @va.
 */
extern bfdev_matrix_t *
bfdev_matrix_sub(const bfdev_alloc_t *alloc, const bfdev_matrix_t *va,
                 const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_mul() - multiplying two matrices.
 * @va: first factor matrix.
 * @vb: second factor matrix.
 */
extern bfdev_matrix_t *
bfdev_matrix_mul(const bfdev_alloc_t *alloc, const bfdev_matrix_t *va,
                 const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_copy() - copy a new matrix.
 * @var: variables to copy.
 */
extern bfdev_matrix_t *
bfdev_matrix_copy(const bfdev_alloc_t *alloc,
                  const bfdev_matrix_t *var);

/**
 * bfdev_matrix_create() - create a new matrix.
 * @row: number of rows in the matrix.
 * @col: number of columns in the matrix.
 */
extern bfdev_matrix_t *
bfdev_matrix_create(const bfdev_alloc_t *alloc,
                    unsigned int row, unsigned int col);

/**
 * bfdev_matrix_destory() - destory a matrix.
 * @var: variables to destory.
 */
extern void
bfdev_matrix_destory(const bfdev_alloc_t *alloc,
                     const bfdev_matrix_t *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MATRIX_H_ */
