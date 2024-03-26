/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MATRIX_H_
#define _BFDEV_MATRIX_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>
#include <bfdev/array.h>

BFDEV_BEGIN_DECLS

#define BFDEV_MATRIX_TYPE bfdev_uw_t
#define BFDEV_MATRIX_BITS BFDEV_BITS_PER_LONG
#define BFDEV_MATRIX_SIZE BFDEV_BYTES_PER_LONG

typedef struct bfdev_matrix bfdev_matrix_t;

struct bfdev_matrix {
    const bfdev_alloc_t *alloc;
    unsigned int row;
    unsigned int col;
    bfdev_array_t value;
};

#define BFDEV_MATRIX_STATIC(ALLOC) { \
    .value = BFDEV_ARRAY_STATIC(ALLOC, BFDEV_MATRIX_SIZE), \
    .alloc = (ALLOC), \
}

#define BFDEV_MATRIX_INIT(alloc) \
    (bfdev_matrix_t) BFDEV_MATRIX_STATIC(alloc)

#define BFDEV_DEFINE_MATRIX(name, alloc) \
    bfdev_matrix_t name = BFDEV_MATRIX_INIT(alloc)

static inline void
bfdev_matrix_init(bfdev_matrix_t *matrix, const bfdev_alloc_t *alloc)
{
    *matrix = BFDEV_MATRIX_INIT(alloc);
}

/**
 * bfdev_matrix_add() - adding two matrices.
 * @va: first addend matrix.
 * @vb: second addend matrix.
 */
extern int
bfdev_matrix_add(bfdev_matrix_t *dest,
                 const bfdev_matrix_t *va, const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_sub() - subtracting two matrices.
 * @va: minuend; matrix to subtract from.
 * @vb: subtrahend; matrix to subtract from @va.
 */
extern int
bfdev_matrix_sub(bfdev_matrix_t *dest,
                 const bfdev_matrix_t *va, const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_mul() - multiplying two matrices.
 * @va: first factor matrix.
 * @vb: second factor matrix.
 */
extern int
bfdev_matrix_mul(bfdev_matrix_t *dest,
                 const bfdev_matrix_t *va, const bfdev_matrix_t *vb);

/**
 * bfdev_matrix_set() - copy data form matrices.
 * @dest: destination matrix.
 * @src: source matrix.
 */
extern int
bfdev_matrix_set(bfdev_matrix_t *dest, const bfdev_matrix_t *src);

/**
 * bfdev_matrix_import() - import data to a matrix.
 * @var: matrix to import data into.
 * @buffer: data buffer to import data.
 * @row: number of rows in the @buffer.
 * @col: number of columns in the @buffer.
 */
extern int
bfdev_matrix_import(bfdev_matrix_t *var, const BFDEV_MATRIX_TYPE *buffer,
                    unsigned int row, unsigned int col);

/**
 * bfdev_matrix_data() - indexing data form a matrix.
 * @var: matrix to index data.
 * @row: index of rows in the @var.
 * @col: index of columns in the @var.
 */
extern const BFDEV_MATRIX_TYPE *
bfdev_matrix_data(const bfdev_matrix_t *var,
                  unsigned int row, unsigned int col);

/**
 * bfdev_matrix_release() - release a matrix.
 * @var: matrix to release.
 */
extern void
bfdev_matrix_release(bfdev_matrix_t *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MATRIX_H_ */
