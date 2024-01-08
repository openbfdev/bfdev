/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/matrix.h>
#include <export.h>

#define GENERIC_MATRIX_ADDSUB(name, operate)        \
export bfdev_matrix_t *                             \
bfdev_matrix_##name(const bfdev_alloc_t *alloc,     \
                    const bfdev_matrix_t *va,       \
                    const bfdev_matrix_t *vb)       \
{                                                   \
    bfdev_matrix_t *result;                         \
    unsigned int row, col;                          \
    unsigned int size, count;                       \
                                                    \
    row = va->row;                                  \
    col = va->col;                                  \
                                                    \
    if (row != vb->row || col != vb->col)           \
        return NULL;                                \
                                                    \
    result = bfdev_matrix_create(alloc, row, col);  \
    if (bfdev_unlikely(!result))                    \
        return NULL;                                \
                                                    \
    size = row * col;                               \
    for (count = 0; count < size; ++count) {        \
        result->values[count]                       \
            = va->values[count] operate             \
              vb->values[count];                    \
    }                                               \
                                                    \
    return result;                                  \
}

GENERIC_MATRIX_ADDSUB(add, +)
GENERIC_MATRIX_ADDSUB(sub, -)

export bfdev_matrix_t *
bfdev_matrix_mul(const bfdev_alloc_t *alloc,
                 const bfdev_matrix_t *va,
                 const bfdev_matrix_t *vb)
{
    bfdev_matrix_t *result;
    unsigned int row, col;
    unsigned int count;
    long value;

    if (va->col != vb->row)
        return NULL;

    result = bfdev_matrix_create(alloc, va->row, vb->col);
    if (bfdev_unlikely(!result))
        return NULL;

    const long (*ada)[va->col] = (void *)va->values;
    const long (*adb)[vb->col] = (void *)vb->values;
    long (*adr)[vb->col] = (void *)result->values;

    /* Reordering optimize */
    for (row = 0; row < va->row; ++row) {
        for (col = 0; col < va->col; ++col) {
            value = ada[row][col];
            for (count = 0; count < vb->col; ++count)
                adr[row][count] += value * adb[col][count];
        }
    }

    return result;
}

export bfdev_matrix_t *
bfdev_matrix_copy(const bfdev_alloc_t *alloc,
                  const bfdev_matrix_t *var)
{
    bfdev_matrix_t *result;
    unsigned int row, col;

    row = var->row;
    col = var->col;

    result = bfdev_matrix_create(alloc, row, col);
    if (bfdev_unlikely(!result))
        return NULL;

    memcpy(result->values, var->values,
           sizeof(*var->values) * row * col);

    return result;
}

export bfdev_matrix_t *
bfdev_matrix_create(const bfdev_alloc_t *alloc,
                    unsigned int row, unsigned int col)
{
    bfdev_matrix_t *var;

    var = bfdev_zalloc(alloc, sizeof(*var) + sizeof(*var->values) * row * col);
    if (bfdev_unlikely(!var))
        return NULL;

    var->row = row;
    var->col = col;

    return var;
}

export void
bfdev_matrix_destory(const bfdev_alloc_t *alloc,
                     const bfdev_matrix_t *var)
{
    bfdev_free(alloc, var);
}
