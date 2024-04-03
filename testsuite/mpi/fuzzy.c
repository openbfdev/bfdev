/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdlib.h>
#include <bfdev/mpi.h>
#include <testsuite.h>

#define TEST_LOOP 100
#define TEST_SIZE 512

static inline int
filling_random(bfdev_mpi_t *var, unsigned int length)
{
    BFDEV_MPI_TYPE *buff;
    size_t size, count;
    int retval;

    size = sizeof(*buff) * length;
    buff = bfdev_malloc(NULL, size);
    if (bfdev_unlikely(!buff))
        return -BFDEV_ENOMEM;

    for (count = 0; count < size; ++count)
        ((char *)buff)[count] = rand();

    retval = bfdev_mpi_import(var, buff, length, buff[0] & 1);
    bfdev_free(NULL, buff);

    return retval;
}

TESTSUITE(
    "mpi:add", NULL, NULL,
    "mpi add fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    if ((retval = filling_random(&va, TEST_SIZE)) ||
        (retval = filling_random(&vb, TEST_SIZE)))
        goto failed;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_mpi_add(&va, &va, &vb);
        if (retval)
            goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}

TESTSUITE(
    "mpi:sub", NULL, NULL,
    "mpi sub fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    if ((retval = filling_random(&va, TEST_SIZE)) ||
        (retval = filling_random(&vb, TEST_SIZE)))
        goto failed;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_mpi_sub(&va, &va, &vb);
        if (retval)
            goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}

TESTSUITE(
    "mpi:mul", NULL, NULL,
    "mpi mul fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    if ((retval = filling_random(&va, TEST_SIZE)) ||
        (retval = filling_random(&vb, TEST_SIZE)))
        goto failed;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_mpi_mul(&result, &va, &vb);
        if (retval)
            goto failed;
    }

failed:
    bfdev_mpi_release(&result);
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}

TESTSUITE(
    "mpi:div", NULL, NULL,
    "mpi div fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    if ((retval = filling_random(&va, TEST_SIZE)) ||
        (retval = filling_random(&vb, TEST_SIZE)))
        goto failed;

    retval = bfdev_mpi_seti(&vb, 1);
    if (retval)
        goto failed;

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_mpi_div(&result, &result, &va, &vb);
        if (retval)
            goto failed;
    }

failed:
    bfdev_mpi_release(&result);
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);
    return retval;
}
