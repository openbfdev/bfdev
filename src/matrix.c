/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/matrix.h>
#include <export.h>

#define GENERIC_MATRIX_ADDSUB(name, operate)            \
export struct bfdev_matrix *                            \
bfdev_matrix_##name(const struct bfdev_alloc *alloc,    \
                    const struct bfdev_matrix *va,      \
                    const struct bfdev_matrix *vb)      \
{                                                       \
    struct bfdev_matrix *result;                        \
    unsigned int row, col;                              \
    unsigned int size, count;                           \
                                                        \
    row = va->row;                                      \
    col = va->col;                                      \
                                                        \
    if (row != vb->row || col != vb->col)               \
        return NULL;                                    \
                                                        \
    result = bfdev_matrix_create(alloc, row, col);      \
    if (bfdev_unlikely(!result))                        \
        return NULL;                                    \
                                                        \
    size = row * col;                                   \
    for (count = 0; count < size; ++count) {            \
        result->values[count]                           \
            = va->values[count] operate                 \
              vb->values[count];                        \
    }                                                   \
                                                        \
    return result;                                      \
}

GENERIC_MATRIX_ADDSUB(add, +)
GENERIC_MATRIX_ADDSUB(sub, -)

export struct bfdev_matrix *
bfdev_matrix_mul(const struct bfdev_alloc *alloc,
                 const struct bfdev_matrix *va,
                 const struct bfdev_matrix *vb)
{
    struct bfdev_matrix *result;
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

    for (row = 0; row < va->row; ++row) {
        for (col = 0; col < va->col; ++col) {
            value = ada[row][col];
            for (count = 0; count < vb->col; ++count)
                adr[row][count] += value * adb[col][count];
        }
    }

    return result;
}

export struct bfdev_matrix *
bfdev_matrix_copy(const struct bfdev_alloc *alloc,
                  const struct bfdev_matrix *var)
{
    struct bfdev_matrix *result;
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

export struct bfdev_matrix *
bfdev_matrix_create(const struct bfdev_alloc *alloc,
                    unsigned int row, unsigned int col)
{
    struct bfdev_matrix *var;

    var = bfdev_zalloc(alloc, sizeof(*var) + sizeof(*var->values) * row * col);
    if (bfdev_unlikely(!var))
        return NULL;

    var->row = row;
    var->col = col;

    return var;
}

export void
bfdev_matrix_destory(const struct bfdev_alloc *alloc,
                     const struct bfdev_matrix *var)
{
    bfdev_free(alloc, var);
}
