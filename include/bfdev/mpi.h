/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
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
    bfdev_bool plus;
};

#define BFDEV_MPI_STATIC(ALLOC) { \
    .alloc = (ALLOC), .plus = bfdev_true, \
    .value = BFDEV_ARRAY_STATIC(ALLOC, BFDEV_MPI_SIZE), \
}

#define BFDEV_MPI_INIT(alloc) \
    (bfdev_mpi_t) BFDEV_MPI_STATIC(alloc)

#define BFDEV_DEFINE_MPI(name, alloc) \
    bfdev_mpi_t name = BFDEV_MPI_INIT(alloc)

static inline void
bfdev_mpi_init(bfdev_mpi_t *mpi, const bfdev_alloc_t *alloc)
{
    *mpi = BFDEV_MPI_INIT(alloc);
}

static inline unsigned long
bfdev_mpi_length(const bfdev_mpi_t *mpi)
{
    return bfdev_array_index(&mpi->value);
}

static inline bfdev_size_t
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

extern bfdev_bool
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
                 unsigned long length, bfdev_bool sign);

extern const BFDEV_MPI_TYPE *
bfdev_mpi_data(const bfdev_mpi_t *var, bfdev_bool *sign);

extern void
bfdev_mpi_release(bfdev_mpi_t *var);

static inline bfdev_mpi_t *
bfdev_mpi_create(const bfdev_alloc_t *alloc)
{
    bfdev_mpi_t *var;

    var = bfdev_malloc(alloc, sizeof(*var));
    if (bfdev_unlikely(!var))
        return BFDEV_NULL;
    bfdev_mpi_init(var, alloc);

    return var;
}

static inline void
bfdev_mpi_destroy(bfdev_mpi_t *var)
{
    bfdev_mpi_release(var);
    bfdev_free(var->alloc, var);
}

BFDEV_DEFINE_CLASS(bfdev_mpi, bfdev_mpi_t *,
    bfdev_mpi_create(alloc),
    bfdev_mpi_destroy(_T),
    const bfdev_alloc_t *alloc
)

BFDEV_END_DECLS

#endif /* _BFDEV_MPI_H_ */
