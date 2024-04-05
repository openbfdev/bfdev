/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdlib.h>
#include <time.h>
#include <bfdev/mpi.h>
#include <testsuite.h>

#define TEST_LOOP 100
#define TEST_SIZE 512

static inline int
filling_random(bfdev_mpi_t *var)
{
    BFDEV_MPI_TYPE *buff;
    unsigned int length;
    size_t size, count;
    int retval;

    length = ((unsigned int)rand() % TEST_SIZE) | 1;
    size = sizeof(*buff) * length;

    buff = bfdev_malloc(NULL, size);
    if (bfdev_unlikely(!buff))
        return -BFDEV_ENOMEM;

    for (count = 0; count < size; ++count)
        ((char *)buff)[count] = rand();

    retval = bfdev_mpi_import(var, buff, length, rand() & 1);
    bfdev_free(NULL, buff);

    return retval;
}

TESTSUITE(
    "mpi:addsub", NULL, NULL,
    "mpi addsub fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    srand(time(NULL));

    retval = -BFDEV_ENOERR;
    for (count = 0; count < TEST_LOOP; ++count) {
        if ((retval = filling_random(&va)) ||
            (retval = filling_random(&vb)))
            goto failed;

        retval = bfdev_mpi_add(&result, &va, &vb);
        if (retval)
            goto failed;

        retval = bfdev_mpi_sub(&result, &result, &vb);
        if (retval)
            goto failed;

        if (bfdev_mpi_cmp(&result, &va)) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

failed:
    bfdev_mpi_release(&result);
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}

TESTSUITE(
    "mpi:muldiv", NULL, NULL,
    "mpi muldiv fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    srand(time(NULL));

    retval = -BFDEV_ENOERR;
    for (count = 0; count < TEST_LOOP; ++count) {
        if ((retval = filling_random(&va)) ||
            (retval = filling_random(&vb)))
            goto failed;

        /* Prevent division by zero */
        retval = bfdev_mpi_bseti(&vb, 0);
        if (retval)
            goto failed;

        retval = bfdev_mpi_mul(&result, &va, &vb);
        if (retval)
            goto failed;

        retval = bfdev_mpi_div(&result, &result, &result, &vb);
        if (retval)
            goto failed;

        if (bfdev_mpi_cmp(&result, &va)) {
            retval = -BFDEV_EFAULT;
            goto failed;
        }
    }

failed:
    bfdev_mpi_release(&result);
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}
