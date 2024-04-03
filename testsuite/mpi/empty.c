/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/mpi.h>
#include <testsuite.h>

TESTSUITE(
    "mpi:add", NULL, NULL,
    "mpi add empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_add(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:sub", NULL, NULL,
    "mpi sub empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_sub(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:mul", NULL, NULL,
    "mpi mul empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_mul(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:div", NULL, NULL,
    "mpi div empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_seti(&vb, 1);
    if (retval)
        goto failed;

    retval = bfdev_mpi_div(&va, &va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:and", NULL, NULL,
    "mpi bit and empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_and(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:or", NULL, NULL,
    "mpi bit or empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_or(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:xor", NULL, NULL,
    "mpi bit xor empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);
    BFDEV_DEFINE_MPI(vb, NULL);

    retval = bfdev_mpi_xor(&va, &va, &vb);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);
    bfdev_mpi_release(&vb);

    return retval;
}

TESTSUITE(
    "mpi:shl", NULL, NULL,
    "mpi shift left empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);

    retval = bfdev_mpi_shli(&va, &va, 1);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);

    return retval;
}

TESTSUITE(
    "mpi:shr", NULL, NULL,
    "mpi shift right empty test"
) {
    int retval;

    BFDEV_DEFINE_MPI(va, NULL);

    retval = bfdev_mpi_shri(&va, &va, 1);
    if (retval)
        goto failed;

    retval = bfdev_mpi_cmpi(&va, 0);
    if (retval) {
        retval = -BFDEV_EFAULT;
        goto failed;
    }

failed:
    bfdev_mpi_release(&va);

    return retval;
}
