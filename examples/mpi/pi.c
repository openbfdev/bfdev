/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "mpi-pi"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/mpi.h>
#include <bfdev/log.h>
#include "../time.h"

#define TEST_LEN 10000
#define TEST_SIZE (TEST_LEN / 4 + 1)
#define TEST_LOOP (TEST_LEN / 1.39793 + 1)

int main(int argc, const char *argv[])
{
    bfdev_mpi_t *vw, *vs, *vv, *vq;
    unsigned int k;
    int retval;

    if (!((vw = bfdev_mpi_create(NULL)) &&
          (vs = bfdev_mpi_create(NULL)) &&
          (vv = bfdev_mpi_create(NULL)) &&
          (vq = bfdev_mpi_create(NULL))))
        return 1;

    if ((retval = bfdev_mpi_set(vw,  16 * 5)) ||
        (retval = bfdev_mpi_set(vv, 239 * 4)) ||
        (retval = bfdev_mpi_set(vq, 10000)))
        return retval;

    for (k = 0; k < TEST_SIZE; ++k) {
        if ((retval = bfdev_mpi_mul(vw, vw, vq)) ||
            (retval = bfdev_mpi_mul(vv, vv, vq)))
            return retval;
    }

    bfdev_log_info("Calculate PI %d:\n", TEST_LEN);
    EXAMPLE_TIME_STATISTICAL(
        for (k = 1; k <= TEST_LOOP; ++k) {
            if ((retval = bfdev_mpi_divi(vw, vw, vw, 25)) ||
                (retval = bfdev_mpi_divi(vv, vv, vv, 239 * 239)) ||
                (retval = bfdev_mpi_sub(vq, vw, vv)) ||
                (retval = bfdev_mpi_divi(vq, vq, vq, 2 * k - 1)))
                return retval;

            if (k & 1)
                retval = bfdev_mpi_add(vs, vs, vq);
            else
                retval = bfdev_mpi_sub(vs, vs, vq);

            if (retval)
                return retval;
        }
        0;
    );

    bfdev_mpi_destory(vw);
    bfdev_mpi_destory(vs);
    bfdev_mpi_destory(vv);
    bfdev_mpi_destory(vq);

    return 0;
}
