/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _MPI_PRINT_H_
#define _MPI_PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/mpi.h>

static inline char *
print_num(const bfdev_mpi_t *var, unsigned int base)
{
    BFDEV_DEFINE_ARRAY(stack, NULL, sizeof(char));
    unsigned long count;
    char *buffer, *str;
    size_t size;
    int retval;

    BFDEV_DEFINE_MPI(value, NULL);
    BFDEV_DEFINE_MPI(taken, NULL);

    retval = bfdev_mpi_set(&value, var);
    if (retval)
        return NULL;

    while (bfdev_mpi_cmpi(&value, 0)) {
        const BFDEV_MPI_TYPE *walk;

        retval = bfdev_mpi_divi(&value, &taken, &value, base);
        if (retval)
            return NULL;

        walk = bfdev_mpi_data(&taken, NULL);
        if (!walk)
            return NULL;

        buffer = bfdev_array_push(&stack, 1);
        if (!buffer)
            return NULL;

        if (*walk < 10)
            *buffer = '0' + *walk;
        else
            *buffer = 'A' + *walk - 10;
    }

    size = bfdev_array_size(&stack);
    if (!size) {
        buffer = bfdev_array_push(&stack, 1);
        if (!buffer)
            return NULL;

        *buffer = '0';
    }

    if (!var->plus) {
        buffer = bfdev_array_push(&stack, 1);
        if (!buffer)
            return NULL;

        *buffer = '-';
    }

    size = bfdev_array_size(&stack);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;

    for (count = 0; count < size; ++count) {
        str = bfdev_array_pop(&stack, 1);
        buffer[count] = *str;
    }

    buffer[count] = '\0';
    bfdev_array_release(&stack);

    bfdev_mpi_release(&taken);
    bfdev_mpi_release(&value);

    return buffer;
}

#endif /* _MPI_PRINT_H_ */
