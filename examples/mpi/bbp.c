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

#define TEST_LEN 10000

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
    bfdev_mpi_t *vv[4], *vq;
    unsigned int k, i, v;
    int retval;

    if (!((vv[0] = bfdev_mpi_create(NULL)) &&
          (vv[1] = bfdev_mpi_create(NULL)) &&
          (vv[2] = bfdev_mpi_create(NULL)) &&
          (vv[3] = bfdev_mpi_create(NULL)) &&
          (vq = bfdev_mpi_create(NULL))))
        return 1;

    bfdev_log_info("Calculate BBP %d:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (i = 0; i < 4; ++i) {
            retval = bfdev_mpi_seti(vv[i], 0);
            if (retval)
                return retval;

            v = factor1[i] + (TEST_LEN << 2);
            retval = bfdev_mpi_bseti(vv[i], v);
            if (retval)
                return retval;

            v = factor2[i];
            retval = bfdev_mpi_divi(vv[i], vv[i], vv[i], v);
            if (retval)
                return retval;

            if (i)
                retval = bfdev_mpi_sub(vq, vq, vv[i]);
            else
                retval = bfdev_mpi_add(vq, vq, *vv);

            if (retval)
                return retval;
        }

        for (k = 1; k < TEST_LEN; ++k) {
            for (i = 0; i < 4; ++i) {
                retval = bfdev_mpi_seti(vv[i], 0);
                if (retval)
                    return retval;

                v = factor1[i] + ((TEST_LEN - k) << 2);
                retval = bfdev_mpi_bseti(vv[i], v);
                if (retval)
                    return retval;

                v = factor2[i] | (k << 3);
                retval = bfdev_mpi_divi(vv[i], vv[i], vv[i], v);
                if (retval)
                    return retval;

                if (i)
                    retval = bfdev_mpi_sub(vq, vq, vv[i]);
                else
                    retval = bfdev_mpi_add(vq, vq, *vv);

                if (retval)
                    return retval;
            }
        }
        0;
    );

    bfdev_mpi_destory(vv[0]);
    bfdev_mpi_destory(vv[1]);
    bfdev_mpi_destory(vv[2]);
    bfdev_mpi_destory(vv[3]);
    bfdev_mpi_destory(vq);

    return 0;
}
