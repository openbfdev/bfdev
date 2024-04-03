/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/mpi.h>
#include "../time.h"
#include "helper.h"

#define TEST_LOOP 10000
#define PRINT_RESULT 1

int main(int argc, const char *argv[])
{
    unsigned int count;
    char *result;
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);
    BFDEV_DEFINE_MPI(vc, NULL);

    bfdev_mpi_seti(&va, 0);
    bfdev_mpi_seti(&vb, 0);
    vb.plus = 0;

    retval = bfdev_mpi_cmp(&va, &vb);
    printf("%d\n", retval);

    if ((retval = bfdev_mpi_seti(&va, 1)) ||
        (retval = bfdev_mpi_seti(&vb, 0)) ||
        (retval = bfdev_mpi_seti(&vc, 0)))
        return retval;

    EXAMPLE_TIME_STATISTICAL(
        for (count = 0; count < TEST_LOOP - 1; ++count) {
            if ((retval = bfdev_mpi_add(&vc, &va, &vb)) ||
                (retval = bfdev_mpi_set(&vb, &va)) ||
                (retval = bfdev_mpi_set(&va, &vc)))
                return retval;
        }
        0;
    );

#if PRINT_RESULT
    result = print_num(&va, 10);
    if (!result)
        return 1;

    puts(result);
    free(result);
#endif

    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    bfdev_mpi_release(&vc);

    return 0;
}
