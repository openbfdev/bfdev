/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "mpi-bbp"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/mpi.h>
#include <bfdev/log.h>
#include "../time.h"
#include "helper.h"

#define TEST_LEN 10000
#define PRINT_RESULT 0

static BFDEV_MPI_TYPE
factor1[] = {
    2, 1, 0, 0,
};

static BFDEV_MPI_TYPE
factor2[] = {
    1, 4, 5, 6,
};

int main(int argc, const char *argv[])
{
    bfdev_mpi_t vq, vv[4];
    unsigned int k, i, v;
    int retval;

    bfdev_mpi_init(&vq, NULL);
    bfdev_mpi_init(&vv[0], NULL);
    bfdev_mpi_init(&vv[1], NULL);
    bfdev_mpi_init(&vv[2], NULL);
    bfdev_mpi_init(&vv[3], NULL);

    bfdev_log_info("Convergence BBP %d:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (i = 0; i < 4; ++i) {
            retval = bfdev_mpi_seti(&vv[i], 0);
            if (retval)
                return retval;

            v = factor1[i] + (TEST_LEN << 2);
            retval = bfdev_mpi_bseti(&vv[i], v);
            if (retval)
                return retval;

            v = factor2[i];
            retval = bfdev_mpi_divi(&vv[i], &vv[i], &vv[i], v);
            if (retval)
                return retval;

            if (i)
                retval = bfdev_mpi_sub(&vq, &vq, &vv[i]);
            else
                retval = bfdev_mpi_add(&vq, &vq, vv);

            if (retval)
                return retval;
        }

        for (k = 1; k < TEST_LEN; ++k) {
            for (i = 0; i < 4; ++i) {
                retval = bfdev_mpi_seti(&vv[i], 0);
                if (retval)
                    return retval;

                v = factor1[i] + ((TEST_LEN - k) << 2);
                retval = bfdev_mpi_bseti(&vv[i], v);
                if (retval)
                    return retval;

                v = factor2[i] | (k << 3);
                retval = bfdev_mpi_divi(&vv[i], &vv[i], &vv[i], v);
                if (retval)
                    return retval;

                if (i) {
                    retval = bfdev_mpi_sub(&vq, &vq, &vv[i]);
                    if (retval)
                        return retval;
                } else {
                    retval = bfdev_mpi_add(&vq, &vq, vv);
                    if (retval)
                        return retval;
                }
            }
        }
        0;
    );

#if PRINT_RESULT
    char *result;

    result = print_num(&vq, 16);
    if (!result)
        return 1;

    printf("%c.", *result);
    puts(result + 1);
    free(result);
#endif

    bfdev_mpi_release(&vv[0]);
    bfdev_mpi_release(&vv[1]);
    bfdev_mpi_release(&vv[2]);
    bfdev_mpi_release(&vv[3]);
    bfdev_mpi_release(&vq);

    return 0;
}
