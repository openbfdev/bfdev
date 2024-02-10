/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MPI_H_
#define _BFDEV_MPI_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>
#include <bfdev/allocator.h>
#include <bfdev/array.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_MPI_TYPE
# define BFDEV_MPI_TYPE unsigned long
#endif

typedef struct bfdev_mpi bfdev_mpi_t;

struct bfdev_mpi {
    bfdev_array_t value;
    bool plus;
};

extern int
bfdev_mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_add(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb);

extern int
bfdev_mpi_sub(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb);

extern int
bfdev_mpi_mul(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb);

extern int
bfdev_mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_addi(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_subi(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_muli(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_set(bfdev_mpi_t *dest, BFDEV_MPI_TYPE val);

extern int
bfdev_mpi_copy(bfdev_mpi_t *dest, const bfdev_mpi_t *src);

extern bfdev_mpi_t *
bfdev_mpi_create(const bfdev_alloc_t *alloc);

extern void
bfdev_mpi_destory(bfdev_mpi_t *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MPI_H_ */
