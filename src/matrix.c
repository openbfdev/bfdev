/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/matrix.h>
#include <export.h>

static inline void
matrix_zero(BFDEV_MATRIX_TYPE *var, unsigned int size)
{
    memset(var, 0, size * BFDEV_MATRIX_SIZE);
}

static inline void
matrix_copy(BFDEV_MATRIX_TYPE *dest, const BFDEV_MATRIX_TYPE *src,
            unsigned int size)
{
    memcpy(dest, src, size * BFDEV_MATRIX_SIZE);
}

#define GENERIC_MATRIX_ADDSUB(name, operate)            \
export int                                              \
bfdev_matrix_##name(bfdev_matrix_t *dest,               \
                    const bfdev_matrix_t *va,           \
                    const bfdev_matrix_t *vb)           \
{                                                       \
    unsigned int row, col;                              \
    unsigned int size, count;                           \
    int retval;                                         \
                                                        \
    row = va->row;                                      \
    col = va->col;                                      \
                                                        \
    if (row != vb->row || col != vb->col)               \
        return -BFDEV_EINVAL;                           \
                                                        \
    size = row * col;                                   \
    retval = bfdev_array_resize(&dest->value, size);    \
    if (bfdev_unlikely(retval))                         \
        return retval;                                  \
                                                        \
    dest->row = row;                                    \
    dest->col = col;                                    \
                                                        \
    const BFDEV_MATRIX_TYPE *ada;                       \
    const BFDEV_MATRIX_TYPE *adb;                       \
    BFDEV_MATRIX_TYPE *adr;                             \
                                                        \
    ada = bfdev_array_data(&va->value, 0);              \
    adb = bfdev_array_data(&vb->value, 0);              \
    adr = bfdev_array_data(&dest->value, 0);            \
                                                        \
    for (count = 0; count < size; ++count)              \
        adr[count] = ada[count] operate adb[count];     \
                                                        \
    return -BFDEV_ENOERR;                               \
}

GENERIC_MATRIX_ADDSUB(add, +)
GENERIC_MATRIX_ADDSUB(sub, -)

export int
bfdev_matrix_mul(bfdev_matrix_t *dest,
                 const bfdev_matrix_t *va, const bfdev_matrix_t *vb)
{
    BFDEV_DEFINE_MATRIX(buffer, dest->alloc);
    bfdev_matrix_t *rename;
    unsigned int row, col;
    unsigned int size, count;
    long value;
    int retval;

    if (va->col != vb->row)
        return -BFDEV_EINVAL;

    rename = NULL;
    if (dest == va || dest == vb) {
        rename = dest;
        dest = &buffer;
    }

    size = va->row * vb->col;
    retval = bfdev_array_resize(&dest->value, size);
    if (bfdev_unlikely(retval))
        return retval;

    dest->row = va->row;
    dest->col = vb->col;

    const BFDEV_MATRIX_TYPE (*ada)[va->col];
    const BFDEV_MATRIX_TYPE (*adb)[vb->col];
    BFDEV_MATRIX_TYPE (*adr)[vb->col];

    ada = bfdev_array_data(&va->value, 0);
    adb = bfdev_array_data(&vb->value, 0);
    adr = bfdev_array_data(&dest->value, 0);

    /* Reordering optimize */
    matrix_zero((void *)adr, size);
    for (row = 0; row < va->row; ++row) {
        for (col = 0; col < va->col; ++col) {
            value = ada[row][col];
            for (count = 0; count < vb->col; ++count)
                adr[row][count] += value * adb[col][count];
        }
    }

    if (rename) {
        retval = bfdev_matrix_set(rename, dest);
        bfdev_matrix_release(dest);
    }

    return retval;
}

export int
bfdev_matrix_set(bfdev_matrix_t *dest, const bfdev_matrix_t *src)
{
    unsigned int row, col;
    unsigned int size;
    void *dbuf, *sbuf;
    int retval;

    row = src->row;
    col = src->col;

    size = row * col;
    retval = bfdev_array_resize(&dest->value, size);
    if (bfdev_unlikely(retval))
        return retval;

    dest->row = row;
    dest->col = col;

    sbuf = bfdev_array_data(&src->value, 0);
    dbuf = bfdev_array_data(&dest->value, 0);
    matrix_copy(dbuf, sbuf, size);

    return -BFDEV_ENOERR;
}

export int
bfdev_matrix_import(bfdev_matrix_t *var, const BFDEV_MATRIX_TYPE *buffer,
                    unsigned int row, unsigned int col)
{
    unsigned int size;
    void *dbuf;
    int retval;

    size = row * col;
    retval = bfdev_array_resize(&var->value, size);
    if (bfdev_unlikely(retval))
        return retval;

    var->row = row;
    var->col = col;

    dbuf = bfdev_array_data(&var->value, 0);
    matrix_copy(dbuf, buffer, size);

    return -BFDEV_ENOERR;
}

export const BFDEV_MATRIX_TYPE *
bfdev_matrix_data(const bfdev_matrix_t *var,
                  unsigned int row, unsigned int col)
{
    BFDEV_MATRIX_TYPE *data;
    unsigned int offset;

    if (var->col < row || var->col < col)
        return NULL;

    offset = row * col;
    data = bfdev_array_data(&var->value, offset);

    return data;
}

export void
bfdev_matrix_release(bfdev_matrix_t *var)
{
    bfdev_array_release(&var->value);
    var->row = 0;
    var->col = 0;
}
