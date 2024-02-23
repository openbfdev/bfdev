/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/mpi.h>
#include "helper.h"

#define TEST_LOOP 10000

int main(int argc, const char *argv[])
{
    bfdev_mpi_t *va, *vb, *vc;
    unsigned int count;
    char *result;
    int retval;

    if (!((va = bfdev_mpi_create(NULL)) &&
         (vb = bfdev_mpi_create(NULL)) &&
         (vc = bfdev_mpi_create(NULL))))
        return 1;

    if ((retval = bfdev_mpi_seti(va, 1)) ||
        (retval = bfdev_mpi_seti(vb, 0)) ||
        (retval = bfdev_mpi_seti(vc, 0)))
        return retval;

    for (count = 0; count < TEST_LOOP - 1; ++count) {
        if ((retval = bfdev_mpi_add(vc, va, vb)) ||
            (retval = bfdev_mpi_set(vb, va)) ||
            (retval = bfdev_mpi_set(va, vc)))
            return retval;
    }

    result = print_num(va, 10);
    if (!result)
        return 1;

    puts(result);
    free(result);

    bfdev_mpi_destory(va);
    bfdev_mpi_destory(vb);
    bfdev_mpi_destory(vc);

    return 0;
}
