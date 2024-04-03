/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "mpi-machin"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/mpi.h>
#include <bfdev/log.h>
#include "../time.h"
#include "helper.h"

#define TEST_LEN 10000
#define TEST_SIZE (TEST_LEN / 4 + 1)
#define TEST_LOOP (TEST_LEN / 1.39793 + 1)
#define PRINT_RESULT 0

int main(int argc, const char *argv[])
{
    bfdev_mpi_t vw, vs, vv, vq;
    unsigned int k;
    int retval;

    bfdev_mpi_init(&vw, NULL);
    bfdev_mpi_init(&vs, NULL);
    bfdev_mpi_init(&vv, NULL);
    bfdev_mpi_init(&vq, NULL);

    /**
     * Machin-like formula:
     *  PI = 16arctan(1/5) - 4arctan(1/239)
     *
     * These formulas are used in conjunction with Gregory's
     * series, the Taylor series expansion for arctangent:
     *  arctan(x) = x - (x^3)/3 + (x^5)/5 - (x^7)/7 + ...
     */

    if ((retval = bfdev_mpi_seti(&vw, 16 * 5)) ||
        (retval = bfdev_mpi_seti(&vv, 4 * 239)) ||
        (retval = bfdev_mpi_seti(&vq, 10000)))
        return retval;

    for (k = 0; k < TEST_SIZE; ++k) {
        if ((retval = bfdev_mpi_mul(&vw, &vw, &vq)) ||
            (retval = bfdev_mpi_mul(&vv, &vv, &vq)))
            return retval;
    }

    bfdev_log_info("Convergence Machin %d:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (k = 1; k <= TEST_LOOP; ++k) {
            if ((retval = bfdev_mpi_divi(&vw, &vw, &vw, 5 * 5)) ||
                (retval = bfdev_mpi_divi(&vv, &vv, &vv, 239 * 239)) ||
                (retval = bfdev_mpi_sub(&vq, &vw, &vv)) ||
                (retval = bfdev_mpi_divi(&vq, &vq, &vq, 2 * k - 1)))
                return retval;

            if (k & 1)
                retval = bfdev_mpi_add(&vs, &vs, &vq);
            else
                retval = bfdev_mpi_sub(&vs, &vs, &vq);

            if (retval)
                return retval;
        }
        0;
    );

#if PRINT_RESULT
    char *result;

    result = print_num(&vs, 10);
    if (!result)
        return 1;

    printf("%c.", *result);
    puts(result + 1);
    free(result);
#endif

    bfdev_mpi_release(&vw);
    bfdev_mpi_release(&vs);
    bfdev_mpi_release(&vv);
    bfdev_mpi_release(&vq);

    return 0;
}
