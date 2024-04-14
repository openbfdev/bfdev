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
filling_random(bfdev_mpi_t *var, bool unsign)
{
    BFDEV_MPI_TYPE *buff;
    unsigned int length;
    size_t size, count;
    bool sign;
    int retval;

    length = ((unsigned int)rand() % TEST_SIZE) | 1;
    size = sizeof(*buff) * length;

    buff = bfdev_malloc(NULL, size);
    if (bfdev_unlikely(!buff))
        return -BFDEV_ENOMEM;

    for (count = 0; count < size; ++count)
        ((char *)buff)[count] = rand();

    sign = unsign ? true : rand() & 1;
    retval = bfdev_mpi_import(var, buff, length, sign);
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
        if ((retval = filling_random(&va, false)) ||
            (retval = filling_random(&vb, false)))
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
        if ((retval = filling_random(&va, false)) ||
            (retval = filling_random(&vb, false)))
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

TESTSUITE(
    "mpi:bitmap", NULL, NULL,
    "mpi bitmap fuzzy test"
) {
    unsigned int count;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    srand(time(NULL));

    retval = -BFDEV_ENOERR;
    for (count = 0; count < TEST_LOOP; ++count) {
        if ((retval = filling_random(&va, true)) ||
            (retval = filling_random(&vb, true)))
            goto failed;

        retval = bfdev_mpi_or(&result, &va, &vb);
        if (retval)
            goto failed;

        retval = bfdev_mpi_and(&result, &result, &va);
        if (retval)
            goto failed;

        retval = bfdev_mpi_xor(&va, &va, &vb);
        if (retval)
            goto failed;

        retval = bfdev_mpi_xor(&vb, &vb, &va);
        if (retval)
            goto failed;

        if (bfdev_mpi_cmp(&result, &vb)) {
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
    "mpi:shift", NULL, NULL,
    "mpi shift fuzzy test"
) {
    unsigned int count, shift;
    int retval;

    BFDEV_DEFINE_MPI(result, NULL);
    BFDEV_DEFINE_MPI(va, NULL);

    srand(time(NULL));

    retval = -BFDEV_ENOERR;
    for (count = 0; count < TEST_LOOP; ++count) {
        if ((retval = filling_random(&va, false)))
            goto failed;

        shift = (unsigned int)rand() % TEST_SIZE;
        retval = bfdev_mpi_shli(&result, &va, shift);
        if (retval)
            goto failed;

        retval = bfdev_mpi_shri(&result, &result, shift);
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
    return retval;
}
