/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/macro.h>
#include <bfdev/matrix.h>

#define TEST_BASE 1
#define TEST_MAXN 46

static const BFDEV_MATRIX_TYPE
matrix_power[] = {
    0, 1,
    1, 0,
};

static const BFDEV_MATRIX_TYPE
matrix_fibonacci[] = {
    1, 1,
    1, 0,
};

static int
power(bfdev_matrix_t *result, const bfdev_matrix_t *var,
      unsigned int pow)
{
    BFDEV_DEFINE_MATRIX(tmp, NULL);
    int retval;

    retval = bfdev_matrix_import(result, matrix_power, 2, 2);
    if (bfdev_unlikely(retval))
        return retval;

    retval = bfdev_matrix_set(&tmp, var);
    if (bfdev_unlikely(retval))
        return retval;

    while (pow) {
        if (pow & 1) {
            retval = bfdev_matrix_mul(result, result, &tmp);
            if (bfdev_unlikely(retval))
                return retval;
        }

        retval = bfdev_matrix_mul(&tmp, &tmp, &tmp);
        if (bfdev_unlikely(retval))
            return retval;

        pow >>= 1;
    }

    bfdev_matrix_release(&tmp);
    return 0;
}

int
main(int argc, const char *argv[])
{
    BFDEV_DEFINE_MATRIX(fibonacci, NULL);
    BFDEV_DEFINE_MATRIX(result, NULL);
    unsigned int count;
    int retval;

    retval = bfdev_matrix_import(&fibonacci, matrix_fibonacci, 2, 2);
    if (retval)
        return retval;

    for (count = TEST_BASE; count <= TEST_MAXN; ++count) {
        const BFDEV_MATRIX_TYPE *value;

        retval = power(&result, &fibonacci, count);
        if (retval)
            return retval;

        value = bfdev_matrix_data(&result, 0, 0);
        if (!value)
            return 1;

        printf("matrix fibonacci %02u: %ld\n", count, *value);
    }

    bfdev_matrix_release(&fibonacci);
    bfdev_matrix_release(&result);

    return 0;
}
