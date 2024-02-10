/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/mpi.h>
#include <bfdev/dword.h>
#include <export.h>

#define mpi_len(mpi) bfdev_array_index(&(mpi)->value)
#define mpi_pos(mpi) bfdev_array_data(&(mpi)->value, 0)
#define mpi_neg(mpi) bfdev_array_peek(&(mpi)->value, 1)

static inline void
mpa_zero(BFDEV_MPI_TYPE *dest, unsigned long length)
{
    memset(dest, 0, length * sizeof(*dest));
}

static inline void
mpa_copy(BFDEV_MPI_TYPE *dest, BFDEV_MPI_TYPE *src, unsigned long length)
{
    memcpy(dest, src, length * sizeof(*dest));
}

static inline bool
mpa_addi(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, bool carry)
{
    *ptrs = *ptra + vi;
    carry = *ptrs++ < *ptra++;

    while (--length) {
        *ptrs = *ptra + carry;
        carry = *ptrs++ < *ptra++;
    }

    return carry;
}

static inline bool
mpa_add(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE *ptrb,
        unsigned long cnta, unsigned long cntb, bool carry)
{
    while (cnta && cntb) {
        *ptrs = *ptra + *ptrb++ + carry;
        carry = *ptrs++ < *ptra++;

        cnta--;
        cntb--;
    }

    if (!cnta) {
        cnta = cntb;
        ptra = ptrb;
    }

    while (cnta--) {
        *ptrs = *ptra + carry;
        carry = *ptrs++ < *ptra++;
    }

    return carry;
}

static inline bool
mpa_subi(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, bool borrow)
{
    *ptrs = *ptra - vi;
    borrow = *ptrs++ > *ptra++;

    while (--length) {
        *ptrs = *ptra + borrow;
        borrow = *ptrs++ > *ptra++;
    }

    return borrow;
}

static inline bool
mpa_sub(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE *ptrb,
        unsigned long cnta, unsigned long cntb, bool borrow)
{
    while (cnta && cntb) {
        *ptrs = *ptra - *ptrb++ - borrow;
        borrow = *ptrs++ > *ptra++;

        cnta--;
        cntb--;
    }

    if (!cnta) {
        cnta = cntb;
        ptra = ptrb;
    }

    while (cnta--) {
        *ptrs = *ptra - borrow;
        borrow = *ptrs++ > *ptra++;
    }

    return borrow;
}

static inline BFDEV_MPI_TYPE
mpa_muli(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_dword_umul(vhigh, vlow, *ptra++, vi);
        vlow += carry;
        carry = (vlow < carry) + vhigh;
        *ptrs++ = vlow;
    }

    return carry;
}

static inline BFDEV_MPI_TYPE
mpa_maci(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_dword_umul(vhigh, vlow, *ptra++, vi);
        vlow += carry;
        carry = (vlow < carry) + vhigh;

        vlow += *ptrs;
        carry += vlow < *ptrs;
        *ptrs++ = vlow;
    }

    return carry;
}

static inline void
mpa_mul(BFDEV_MPI_TYPE *ptrs, BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE *ptrb,
        unsigned long cnta, unsigned long cntb)
{
    BFDEV_MPI_TYPE value, carry;
    unsigned long index;

    /**
     * Argument constraints:
     * 1. @cnta >= @cntb.
     * 2. @ptrs != @ptra and @ptrs != @ptrb, i.e. the destination
     *    must be distinct from the multiplier and the multiplicand.
     */

    value = *ptrb;
    if (value > 1)
        carry = mpa_muli(ptrs, ptra, value, cnta, 0);
    else {
        if (value)
            mpa_copy(ptrs, ptra, cnta);
        else
            mpa_zero(ptrs, cnta);

        carry = 0;
    }

    ptrs[cnta] = carry;
    ptrs++;

    for (index = 1; index < cntb; ++index) {
        value = ptrb[index];
        if (value > 1)
            carry = mpa_maci(ptrs, ptra, value, cnta, 0);
        else {
            carry = 0;
            if (value)
                carry = mpa_add(ptrs, ptrs, ptra, cnta, cnta, 0);
        }

        ptrs[cnta] = carry;
        ptrs++;
    }
}

static inline void
mpi_relocation(bfdev_mpi_t *var)
{
    BFDEV_MPI_TYPE *value;

    while (mpi_len(var) > 1) {
        value = bfdev_array_peek(&var->value, 1);
        if (*value)
            break;

        bfdev_array_pop(&var->value, 1);
    }
}

static inline int
mpi_set(bfdev_mpi_t *dest, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;

    bfdev_array_reset(&dest->value);
    value = bfdev_array_push(&dest->value, 1);
    if (bfdev_unlikely(!value))
        return -BFDEV_ENOMEM;

    *value = vi;
    return -BFDEV_ENOERR;
}

static inline int
mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi, bool unsign)
{
    unsigned long length;
    BFDEV_MPI_TYPE *value;

    if (!unsign && !va->plus)
        return -1;

    length = mpi_len(va);
    if (length > 1) {
        if (unsign || va->plus)
            return 1;
        else
            return -1;
    }

    value = mpi_pos(va);
    if (*value != vi) {
        if (*value > vi) {
            if (unsign || va->plus)
                return 1;
            else
                return -1;
        } else {
            if (unsign || va->plus)
                return -1;
            else
                return 1;
        }
    }

    return 0;
}

static inline int
mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb, bool unsign)
{
    BFDEV_MPI_TYPE *ptra, *ptrb;
    unsigned long length, lena, lenb;

    if (!unsign && va->plus != vb->plus) {
        if (va->plus)
            return 1;
        else
            return -1;
    }

    lena = mpi_len(va);
    lenb = mpi_len(vb);

    if (lena != lenb) {
        if (lena > lenb) {
            if (unsign || va->plus)
                return 1;
            else
                return -1;
        } else {
            if (unsign || va->plus)
                return -1;
            else
                return 1;
        }
    }

    ptra = mpi_neg(va);
    ptrb = mpi_neg(vb);

    for (length = lena; length; --length) {
        if (*ptra != *ptrb)
            break;
        ptra--;
        ptrb--;
    }

    if (length) {
        if (*ptra > *ptrb) {
            if (unsign || va->plus)
                return 1;
            else
                return -1;
        } else {
            if (unsign || va->plus)
                return -1;
            else
                return 1;
        }
    }

    return 0;
}

static inline int
mpi_addi(bfdev_mpi_t *dest, const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    bool carry;
    int retval;

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(va);

    carry = mpa_addi(ptrs, ptra, vi, length, false);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_add(bfdev_mpi_t *dest, const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bool carry;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = bfdev_max(cnta, cntb);

    retval = bfdev_array_resize(&dest->value, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(va);
    ptrb = mpi_pos(vb);

    carry = mpa_add(ptrs, ptra, ptrb, cnta, cntb, false);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_subi(bfdev_mpi_t *dest, const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    bool borrow;
    int retval;

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(va);

    borrow = mpa_subi(ptrs, ptra, vi, length, false);
    if (bfdev_unlikely(borrow))
        return -BFDEV_EOVERFLOW;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_sub(bfdev_mpi_t *dest, const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bool borrow;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = bfdev_max(cnta, cntb);

    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(va);
    ptrb = mpi_pos(vb);

    borrow = mpa_sub(ptrs, ptra, ptrb, cnta, cntb, false);
    if (bfdev_unlikely(borrow))
        return -BFDEV_EOVERFLOW;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_muli(bfdev_mpi_t *dest, const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    BFDEV_MPI_TYPE carry;
    unsigned long length;
    int retval;

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(va);

    carry = mpa_muli(ptrs, ptra, vi, length, 0);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_mul(bfdev_mpi_t *dest, const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bfdev_array_t buffer;
    int retval;
    bool nval;

    nval = false;
    if (dest == va || dest == vb)
        buffer = dest->value;
    else {
        bfdev_array_init(&buffer, dest->value.alloc, sizeof(*ptrs));
        nval = true;
    }

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = cnta + cntb;

    retval = bfdev_array_resize(&buffer, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = bfdev_array_data(&buffer, 0);
    ptra = mpi_pos(va);
    ptrb = mpi_pos(vb);

    mpa_mul(ptrs, ptra, ptrb, cnta, cntb);
    if (nval) {
        bfdev_array_release(&dest->value);
        dest->value = buffer;
    }
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    return mpi_cmp(va, vb, false);
}

export int
bfdev_mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    return mpi_cmpi(va, vi, false);
}

export int
bfdev_mpi_add(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb)
{
    int diff, retval;

    if (va->plus == vb->plus) {
        retval = mpi_add(dest, va, vb);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = va->plus;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmp(va, vb, true);
    switch (diff) {
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = true;
            break;

        case 1:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = va->plus;
            break;

        case -1:
            retval = mpi_sub(dest, vb, va);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = vb->plus;
            break;
    }

    return retval;
}

export int
bfdev_mpi_addi(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;
    int diff, retval;

    if (va->plus) {
        retval = mpi_addi(dest, va, vi);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = true;
        return -BFDEV_ENOERR;
    }

    diff = bfdev_mpi_cmpi(va, vi);
    switch (diff) {
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = true;
            break;

        case -1:
            value = mpi_pos(va);
            retval = mpi_set(dest, vi - *value);
            dest->plus = true;
            break;

        case 1:
            retval = mpi_subi(dest, va, vi);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = false;
            break;
    }

    return retval;
}

export int
bfdev_mpi_sub(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb)
{
    int diff, retval;

    if (va->plus != vb->plus) {
        retval = mpi_add(dest, va, vb);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = va->plus;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmp(va, vb, true);
    switch (diff) {
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = true;
            break;

        case 1:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = va->plus;
            break;

        case -1:
            retval = mpi_sub(dest, vb, va);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = !vb->plus;
            break;
    }

    return retval;
}

export int
bfdev_mpi_subi(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;
    int diff, retval;

    if (!va->plus) {
        retval = mpi_addi(dest, va, vi);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = false;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmpi(va, vi, true);
    switch (diff) {
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = true;
            break;

        case -1:
            value = mpi_pos(va);
            retval = mpi_set(dest, vi - *value);
            dest->plus = false;
            break;

        case 1:
            retval = mpi_subi(dest, va, vi);
            if (bfdev_unlikely(retval))
                break;

            dest->plus = va->plus;
            break;
    }

    return retval;
}

export int
bfdev_mpi_mul(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
              const bfdev_mpi_t *vb)
{
    unsigned long cnta, cntb;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);

    if (cnta > cntb) {
        bfdev_swap(vb ,va);
        bfdev_swap(cntb, cnta);
    }

    retval = mpi_mul(dest, va, vb);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = !(va->plus ^ vb->plus);

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_muli(bfdev_mpi_t *dest, const bfdev_mpi_t *va,
               BFDEV_MPI_TYPE vi)
{
    int retval;

    retval = mpi_muli(dest, va, vi);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = va->plus;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_set(bfdev_mpi_t *dest, BFDEV_MPI_TYPE val)
{
    return mpi_set(dest, val);
}

export int
bfdev_mpi_copy(bfdev_mpi_t *dest, const bfdev_mpi_t *src)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    int retval;

    length = mpi_len(src);

    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_pos(dest);
    ptra = mpi_pos(src);

    mpa_copy(ptrs, ptra, length);
    dest->plus = src->plus;

    return -BFDEV_ENOERR;
}

export bfdev_mpi_t *
bfdev_mpi_create(const bfdev_alloc_t *alloc)
{
    bfdev_mpi_t *result;
    BFDEV_MPI_TYPE *array;

    result = bfdev_malloc(alloc, sizeof(*result));
    if (bfdev_unlikely(!result))
        return NULL;

    bfdev_array_init(&result->value, alloc, sizeof(*array));
    array = bfdev_array_push(&result->value, 1);
    if (bfdev_unlikely(!array))
        return NULL;

    result->plus = true;
    *array = 0;

    return result;
}

export void
bfdev_mpi_destory(bfdev_mpi_t *var)
{
    const bfdev_alloc_t *alloc;

    alloc = var->value.alloc;
    bfdev_array_release(&var->value);

    bfdev_free(alloc, var);
}
