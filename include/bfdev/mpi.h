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

#define BFDEV_MPI_TYPE bfdev_uw_t
#define BFDEV_MPI_BITS BFDEV_BITS_PER_LONG
#define BFDEV_MPI_SIZE BFDEV_BYTES_PER_LONG

typedef struct bfdev_mpi bfdev_mpi_t;

struct bfdev_mpi {
    const bfdev_alloc_t *alloc;
    bfdev_array_t value;
    bool plus;
};

static inline unsigned long
bfdev_mpi_length(const bfdev_mpi_t *mpi)
{
    return bfdev_array_index(&mpi->value);
}

static inline size_t
bfdev_mpi_size(const bfdev_mpi_t *mpi)
{
    return bfdev_array_size(&mpi->value);
}

extern int
bfdev_mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_add(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_sub(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_mul(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_div(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_mod(bfdev_mpi_t *rem,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_addi(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_subi(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_muli(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_divi(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_modi(bfdev_mpi_t *rem,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi);

extern int
bfdev_mpi_and(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_or(bfdev_mpi_t *dest,
             const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_xor(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb);

extern int
bfdev_mpi_shli(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift);

extern int
bfdev_mpi_shri(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift);

extern bool
bfdev_mpi_btesti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit);

extern int
bfdev_mpi_bseti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit);

extern int
bfdev_mpi_bclri(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit);

extern int
bfdev_mpi_seti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE val);

extern int
bfdev_mpi_set(bfdev_mpi_t *dest, const bfdev_mpi_t *src);

extern int
bfdev_mpi_import(bfdev_mpi_t *var, const BFDEV_MPI_TYPE *buffer,
                 unsigned long length, bool sign);

extern const BFDEV_MPI_TYPE *
bfdev_mpi_data(const bfdev_mpi_t *var,
               unsigned long index, bool *sign);

extern bfdev_mpi_t *
bfdev_mpi_create(const bfdev_alloc_t *alloc);

extern void
bfdev_mpi_destory(bfdev_mpi_t *var);

BFDEV_END_DECLS

#endif /* _BFDEV_MPI_H_ */
