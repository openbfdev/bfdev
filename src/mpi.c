/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-mpi"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/mpi.h>
#include <bfdev/dword.h>
#include <bfdev/bitmap.h>
#include <export.h>

#define mpi_len(mpi) bfdev_array_index(&(mpi)->value)
#define mpi_val(mpi) bfdev_array_data(&(mpi)->value, 0)
#define mpi_resize(mpi, size) bfdev_array_resize(&(mpi)->value, size)

static const
BFDEV_MPI_TYPE mpi_zero;

/**
 * Multi Precision Array Computing.
 *
 * The following functions are used to
 * calculating raw data within an array.
 */

static inline void
mpa_zero(BFDEV_MPI_TYPE *dest, unsigned long length)
{
    bfport_memset(dest, 0, length * BFDEV_MPI_SIZE);
}

static inline void
mpa_copy(BFDEV_MPI_TYPE *dest, const BFDEV_MPI_TYPE *src,
         unsigned long length)
{
    bfport_memcpy(dest, src, length * BFDEV_MPI_SIZE);
}

static inline int
mpa_cmpi(const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length)
{
    if (length > 1)
        return BFDEV_BT;

    if (*ptra != vi)
        return bfdev_cmp(*ptra > vi);

    return BFDEV_EQ;
}

static inline int
mpa_cmp(const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
        unsigned long lena, unsigned long lenb)
{
    unsigned long length;

    if (lena != lenb)
        return bfdev_cmp(lena > lenb);

    ptra += lena - 1;
    ptrb += lenb - 1;

    for (length = lena; length; --length) {
        if (*ptra != *ptrb)
            break;
        ptra--;
        ptrb--;
    }

    if (length)
        return bfdev_cmp(*ptra > *ptrb);

    return BFDEV_EQ;
}

static inline bool
mpa_addi(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, bool carry)
{
    BFDEV_MPI_TYPE value;

    value = *ptra + vi;
    carry = value < *ptra++;
    *ptrs++ = value;

    while (--length) {
        *ptrs = *ptra + carry;
        carry = *ptrs++ < *ptra++;
    }

    return carry;
}

static inline bool
mpa_add(BFDEV_MPI_TYPE *ptrs,
        const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
        unsigned long cnta, unsigned long cntb, bool carry)
{
    BFDEV_MPI_TYPE value;

    while (cnta && cntb) {
        value = *ptra + *ptrb++ + carry;
        carry = value < *ptra++;
        *ptrs++ = value;

        cnta--;
        cntb--;
    }

    if (!cnta) {
        cnta = cntb;
        ptra = ptrb;
    }

    while (cnta--) {
        value = *ptra + carry;
        carry = value < *ptra++;
        *ptrs++ = value;
    }

    return carry;
}

static inline bool
mpa_subi(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, bool borrow)
{
    BFDEV_MPI_TYPE value;

    value = *ptra - vi;
    borrow = value > *ptra++;
    *ptrs++ = value;

    while (--length) {
        value = *ptra - borrow;
        borrow = value > *ptra++;
        *ptrs++ = value;
    }

    return borrow;
}

static inline bool
mpa_sub(BFDEV_MPI_TYPE *ptrs,
        const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
        unsigned long cnta, unsigned long cntb, bool borrow)
{
    BFDEV_MPI_TYPE value;

    while (cnta && cntb) {
        value = *ptra - *ptrb++ - borrow;
        borrow = value > *ptra++;
        *ptrs++ = value;

        cnta--;
        cntb--;
    }

    while (cnta--) {
        value = *ptra - borrow;
        borrow = value > *ptra++;
        *ptrs++ = value;
    }

    return borrow;
}

static inline BFDEV_MPI_TYPE
mpa_muli(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_umul_ppmm(vhigh, vlow, *ptra++, vi);
        vlow += carry;
        carry = (vlow < carry) + vhigh;
        *ptrs++ = vlow;
    }

    return carry;
}

static inline BFDEV_MPI_TYPE
mpa_maci(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_umul_ppmm(vhigh, vlow, *ptra++, vi);
        vlow += carry;
        carry = (vlow < carry) + vhigh;

        vlow = *ptrs + vlow;
        carry += vlow < *ptrs;
        *ptrs++ = vlow;
    }

    return carry;
}

static inline BFDEV_MPI_TYPE
mpa_msui(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_umul_ppmm(vhigh, vlow, *ptra++, vi);
        vlow += carry;
        carry = (vlow < carry) + vhigh;

        vlow = *ptrs - vlow;
        carry += vlow > *ptrs;
        *ptrs++ = vlow;
    }

    return carry;
}

static inline void
mpa_mul(BFDEV_MPI_TYPE *ptrs,
        const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
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

static inline BFDEV_MPI_TYPE
mpa_divmodi(BFDEV_MPI_TYPE *ptrs,
            const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
            unsigned long length)
{
    BFDEV_MPI_TYPE quot[2], dword[2];
    unsigned long index;

    index = length - 1;
    ptrs += index;
    ptra += index;
    dword[1] = *ptra;

    if (dword[1] >= vi)
        dword[1] = 0;
    else {
        *ptrs-- = 0;
        ptra--;
        length--;
    }

    while (length--) {
        dword[0] = *ptra--;
        bfdev_dword_udiv(quot, dword + 1, dword, vi);
        *ptrs-- = quot[0];
    }

    return dword[1];
}

static inline BFDEV_MPI_TYPE
mpa_modi(const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length)
{
    BFDEV_MPI_TYPE dword[2];
    unsigned long index;

    index = length - 1;
    ptra += index;
    dword[1] = *ptra;

    if (dword[1] >= vi)
        dword[1] = 0;
    else {
        ptra--;
        length--;
    }

    while (length--) {
        dword[0] = *ptra--;
        bfdev_dword_udiv(NULL, dword + 1, dword, vi);
    }

    return dword[1];
}

static inline bool
mpa_divrem(BFDEV_MPI_TYPE *ptrs,
           BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
           unsigned long cnta, unsigned long cntb)
{
    BFDEV_MPI_TYPE dhigh, dlow, value;
    unsigned long index;
    bool limb, borrow;

    /**
     * Argument constraints:
     * 0. @cnta >= @cntb.
     * 1. The most significant bit of the divisor must be set.
     * 2. @ptrs must either not overlap with the input operands at all, or
     *    @ptrs + @cntb >= @ptra must hold true. (This means that it's
     *    possible to put the quotient in the high part of NUM, right after the
     *    remainder in NUM.
     */

    index = cnta - cntb;
    ptra += index;

    value = ptra[cntb - 1];
    dhigh = ptrb[cntb - 1];
    dlow = ptrb[cntb - 2];

    limb = false;
    if (value >= dhigh) {
        if (value > dhigh || mpa_cmp(ptra, ptrb, cntb - 1, cntb - 1) >= 0) {
            borrow = mpa_sub(ptra, ptra, ptrb, cntb, cntb, false);
            BFDEV_BUG_ON(borrow);

            value = ptra[cntb - 1];
            limb = true;
        }
    }

    while (index--) {
        BFDEV_MPI_TYPE quot, carry;
        BFDEV_MPI_TYPE v1, v2;

        ptra--;
        v2 = ptra[cntb];

        if (value == dhigh)
            quot = ~(BFDEV_MPI_TYPE)0UL;
        else {
            BFDEV_MPI_TYPE result[2], dword[2];
            BFDEV_MPI_TYPE rem;

            dword[0] = ptra[cntb - 1];
            dword[1] = value;
            bfdev_dword_udiv(result, &rem, dword, dhigh);

            quot = result[0];
            bfdev_umul_ppmm(v1, value, dlow, quot);

            while (v1 > rem || (v1 == rem && value > ptra[cntb - 2])) {
                quot--;
                rem += dhigh;

                if (rem < dhigh)
                    break;

                v1 -= value < dlow;
                value -= dlow;
            }
        }

        carry = mpa_msui(ptra, ptrb, quot, cntb, 0);
        if (v2 != carry) {
            mpa_add(ptra, ptra, ptrb, cntb, cntb, 0);
            quot--;
        }

        if (ptrs)
            ptrs[index] = quot;
        value = ptra[cntb - 1];
    }

    return limb;
}

/**
 * Multi Precision Integers (unsigned).
 *
 * The following functions are used to
 * calculating unsigned mpi.
 */

static __bfdev_always_inline unsigned long
mpi_relocation(bfdev_mpi_t *var)
{
    BFDEV_MPI_TYPE *value;
    unsigned long length;

    length = 0;
    while (mpi_len(var)) {
        value = bfdev_array_peek(&var->value, 1);
        if (*value)
            break;

        bfdev_array_pop(&var->value, 1);
        length++;
    }

    return length;
}

static inline int
mpi_extend(bfdev_mpi_t *var, unsigned long length)
{
    BFDEV_MPI_TYPE *base;
    unsigned long index;
    int retval;

    index = mpi_len(var);
    if (index >= length)
        return -BFDEV_ENOERR;

    retval = mpi_resize(var, length);
    if (bfdev_unlikely(retval))
        return retval;

    base = bfdev_array_data(&var->value, index);
    mpa_zero(base, length - index);

    return -BFDEV_ENOERR;
}

static inline int
mpi_copy(bfdev_mpi_t *var, const BFDEV_MPI_TYPE *buffer,
         unsigned long length)
{
    BFDEV_MPI_TYPE *ptrs;
    int retval;

    retval = mpi_resize(var, length);
    if (bfdev_unlikely(retval))
        return retval;

    if (length) {
        ptrs = mpi_val(var);
        mpa_copy(ptrs, buffer, length);
        mpi_relocation(var);
    }

    return -BFDEV_ENOERR;
}

static inline int
mpi_seti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;
    int retval;

    retval = mpi_resize(dest, !!vi);
    if (bfdev_unlikely(retval))
        return retval;

    if (vi) {
        value = mpi_val(dest);
        *value = vi;
    }

    return -BFDEV_ENOERR;
}

static inline int
mpi_set(bfdev_mpi_t *dest, const bfdev_mpi_t *src)
{
    BFDEV_MPI_TYPE *ptra;
    unsigned long length;

    if (dest == src)
        return -BFDEV_ENOERR;

    length = mpi_len(src);
    ptra = mpi_val(src);

    return mpi_copy(dest, ptra, length);
}

static inline int
mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;
    unsigned long length;

    length = mpi_len(va);
    value = mpi_val(va);

    if (!length) {
        if (vi)
            return BFDEV_LT;
        return BFDEV_EQ;
    }

    return mpa_cmpi(value, vi, length);
}

static inline int
mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptra, *ptrb;
    unsigned long lena, lenb;

    lena = mpi_len(va);
    lenb = mpi_len(vb);

    if (!lena) {
        if (lenb)
            return BFDEV_LT;
        return BFDEV_EQ;
    }

    if (!lenb)
        return BFDEV_BT;

    ptra = mpi_val(va);
    ptrb = mpi_val(vb);

    return mpa_cmp(ptra, ptrb, lena, lenb);
}

static inline int
mpi_addi(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    bool carry;
    int retval;

    /* parameter check */
    length = mpi_len(va);
    if (!length)
        return mpi_seti(dest, vi);

    retval = mpi_resize(dest, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    carry = mpa_addi(ptrs, ptra, vi, length, false);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_add(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bool carry;
    int retval;

    /* parameter check */
    cnta = mpi_len(va);
    if (!cnta)
        return mpi_set(dest, vb);

    cntb = mpi_len(vb);
    if (!cntb)
        return mpi_set(dest, va);

    /* degrade to addi */
    ptrb = mpi_val(vb);
    if (cntb == 1)
        return mpi_addi(dest, va, *ptrb);

    length = bfdev_max(cnta, cntb);
    retval = mpi_resize(dest, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptra = mpi_val(va);
    ptrs = mpi_val(dest);

    carry = mpa_add(ptrs, ptra, ptrb, cnta, cntb, false);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_subi(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    bool borrow;
    int retval;

    /* parameter check */
    length = mpi_len(va);
    BFDEV_BUG_ON(!length);

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    borrow = mpa_subi(ptrs, ptra, vi, length, false);
    BFDEV_BUG_ON(borrow);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_sub(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb;
    bool borrow;
    int retval;

    /* parameter check */
    cntb = mpi_len(vb);
    if (!cntb)
        return mpi_set(dest, va);

    /* degrade to subi */
    ptrb = mpi_val(vb);
    if (cntb == 1)
        return mpi_subi(dest, va, *ptrb);

    cnta = mpi_len(va);
    BFDEV_BUG_ON(cnta < cntb);

    retval = mpi_resize(dest, cnta);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    borrow = mpa_sub(ptrs, ptra, ptrb, cnta, cntb, false);
    BFDEV_BUG_ON(borrow);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_muli(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    BFDEV_MPI_TYPE carry;
    unsigned long length;
    int retval;

    /* parameter check */
    length = mpi_len(va);
    if (!length || !vi)
        return mpi_seti(dest, 0);

    retval = mpi_resize(dest, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    carry = mpa_muli(ptrs, ptra, vi, length, 0);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_mul(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_DEFINE_MPI(buffer, dest->alloc);
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bfdev_mpi_t *rename;
    int retval;

    /* parameter check */
    cnta = mpi_len(va);
    if (!cnta)
        return mpi_seti(dest, 0);

    cntb = mpi_len(vb);
    if (!cntb)
        return mpi_seti(dest, 0);

    /* degrade to muli */
    ptrb = mpi_val(vb);
    if (cntb == 1)
        return mpi_muli(dest, va, *ptrb);

    rename = NULL;
    if (dest == va && dest == vb) {
        rename = dest;
        dest = &buffer;
    }

    length = cnta + cntb;
    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    mpa_mul(ptrs, ptra, ptrb, cnta, cntb);
    mpi_relocation(dest);

    if (rename) {
        mpi_set(rename, dest);
        bfdev_mpi_release(dest);
    }

    return -BFDEV_ENOERR;
}

static inline int
mpi_divi(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, value;
    unsigned long length;
    int retval;

    /* divide by zero */
    if (bfdev_unlikely(!vi))
        return -BFDEV_EOVERFLOW;

    /* parameter check */
    length = mpi_len(va);
    if (!length) {
        retval = mpi_seti(quot, 0);
        if (retval)
            return retval;

        retval = mpi_seti(rem, 0);
        if (retval)
            return retval;

        return -BFDEV_ENOERR;
    }

    retval = mpi_resize(quot, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(quot);
    ptra = mpi_val(va);

    value = mpa_divmodi(ptrs, ptra, vi, length);
    mpi_relocation(quot);

    if (quot != rem)
        mpi_seti(rem, value);

    return -BFDEV_ENOERR;
}

static inline int
mpi_modi(bfdev_mpi_t *rem,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptra, value;
    unsigned long length;

    /* divide by zero */
    if (bfdev_unlikely(!vi))
        return -BFDEV_EOVERFLOW;

    /* parameter check */
    length = mpi_len(va);
    if (!length)
        return mpi_seti(rem, 0);

    length = mpi_len(va);
    ptra = mpi_val(va);

    value = mpa_modi(ptra, vi, length);
    mpi_seti(rem, value);

    return -BFDEV_ENOERR;
}

static inline int
mpi_div(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_DEFINE_MPI(buffer, quot->alloc);
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb, length;
    bfdev_mpi_t *rename;
    bool limb;
    int retval;

    /* divide by zero */
    cntb = mpi_len(vb);
    if (bfdev_unlikely(!cntb))
        return -BFDEV_EOVERFLOW;

    /* degrade to divi */
    ptrb = mpi_val(vb);
    if (cntb == 1)
        return mpi_divi(quot, rem, va, *ptrb);

    /* parameter check */
    cnta = mpi_len(va);
    if (!cnta) {
        retval = mpi_seti(quot, 0);
        if (retval)
            return retval;

        retval = mpi_seti(rem, 0);
        if (retval)
            return retval;

        return -BFDEV_ENOERR;
    }

    if (rem != va) {
        retval = mpi_set(rem, va);
        if (bfdev_unlikely(retval))
            return retval;
    }

    rename = NULL;
    if (quot == va) {
        rename = quot;
        quot = &buffer;
    }

    length = cnta + cntb;
    retval = mpi_resize(quot, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(quot);
    ptra = mpi_val(rem);

    limb = mpa_divrem(ptrs, ptra, ptrb, cnta, cntb);
    length = cnta - cntb;

    if (limb) {
        ptrs[length] = limb;
        length += 1;
    }

    BFDEV_BUG_ON(mpi_resize(quot, length));
    BFDEV_BUG_ON(mpi_resize(rem, cntb));

    BFDEV_BUG_ON(mpi_relocation(quot));
    mpi_relocation(rem);

    if (rename) {
        mpi_set(rename, quot);
        bfdev_mpi_release(quot);
    }

    return -BFDEV_ENOERR;
}

static inline int
mpi_mod(bfdev_mpi_t *rem,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptra, *ptrb;
    unsigned long cnta, cntb;
    int retval;

    /* divide by zero */
    cntb = mpi_len(vb);
    if (bfdev_unlikely(!cntb))
        return -BFDEV_EOVERFLOW;

    /* degrade to divi */
    ptrb = mpi_val(vb);
    if (cntb == 1)
        return mpi_modi(rem, va, *ptrb);

    /* parameter check */
    cnta = mpi_len(va);
    if (!cnta)
        return mpi_seti(rem, 0);

    if (rem != va) {
        retval = mpi_set(rem, va);
        if (bfdev_unlikely(retval))
            return retval;
    }

    cnta = mpi_len(va);
    ptra = mpi_val(rem);

    mpa_divrem(NULL, ptra, ptrb, cnta, cntb);
    BFDEV_BUG_ON(mpi_resize(rem, cntb));
    mpi_relocation(rem);

    return -BFDEV_ENOERR;
}

static inline int
mpi_and(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb, length;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = bfdev_min(cnta, cntb);

    /* parameter check */
    if (!length)
        return mpi_seti(dest, 0);

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);
    ptrb = mpi_val(vb);

    bfdev_bitmap_and(ptrs, ptra, ptrb, length * BFDEV_MPI_BITS);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_or(bfdev_mpi_t *dest,
       const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb, length;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = bfdev_max(cnta, cntb);

    /* parameter check */
    if (!length)
        return mpi_seti(dest, 0);

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);
    ptrb = mpi_val(vb);

    length = bfdev_min(cnta, cntb);
    bfdev_bitmap_or(ptrs, ptra, ptrb, length * BFDEV_MPI_BITS);

    if (cnta < cntb) {
        bfdev_swap(ptra, ptrb);
        bfdev_swap(cnta, cntb);
    }

    length = cnta - cntb;
    mpa_copy(ptrs + cntb, ptra + cntb, length);
    BFDEV_BUG_ON(mpi_relocation(dest));

    return -BFDEV_ENOERR;
}

static inline int
mpi_xor(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb, length;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);
    length = bfdev_max(cnta, cntb);

    /* parameter check */
    if (!length)
        return mpi_seti(dest, 0);

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);
    ptrb = mpi_val(vb);

    length = bfdev_min(cnta, cntb);
    bfdev_bitmap_xor(ptrs, ptra, ptrb, length * BFDEV_MPI_BITS);

    if (cnta < cntb) {
        bfdev_swap(ptra, ptrb);
        bfdev_swap(cnta, cntb);
    }

    length = cnta - cntb;
    mpa_copy(ptrs + cntb, ptra + cntb, length);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_shli(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length, cnta, rem;
    int retval;

    /* parameter check */
    if (!shift)
        return mpi_set(dest, va);

    cnta = mpi_len(va);
    if (!cnta)
        return mpi_seti(dest, 0);

    rem = BFDEV_BITS_TO_LONG(shift);
    length = cnta + rem;

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    if (dest != va) {
        ptra = mpi_val(va);
        mpa_copy(ptrs, ptra, cnta);
    }

    mpa_zero(ptrs + cnta, rem);
    bfdev_bitmap_shl(ptrs, ptrs, shift, length * BFDEV_MPI_BITS);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_shri(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    int retval;

    /* parameter check */
    if (!shift)
        return mpi_set(dest, va);

    length = mpi_len(va);
    if (!length)
        return mpi_seti(dest, 0);

    retval = mpi_resize(dest, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    bfdev_bitmap_shr(ptrs, ptra, shift, length * BFDEV_MPI_BITS);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

/**
 * Multi Precision Integers (signed).
 *
 * The following function is used to calculate
 * signed mpi and provide export api.
 */

export int
bfdev_mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int diff;

    if (va->plus != vb->plus)
        return bfdev_cmp(va->plus);

    diff = mpi_cmp(va, vb);
    if (diff > 0)
        return bfdev_cmp(va->plus);

    if (diff < 0)
        return bfdev_cmp(!va->plus);

    return BFDEV_EQ;
}

export int
bfdev_mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    if (!va->plus)
        return BFDEV_LT;

    return mpi_cmpi(va, vi);
}

export int
bfdev_mpi_add(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    unsigned long cnta, cntb;
    int diff, retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);

    /* satisfy commutative law */
    if (cnta < cntb)
        bfdev_swap(va, vb);

    if (va->plus == vb->plus) {
        retval = mpi_add(dest, va, vb);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = va->plus;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmp(va, vb);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case BFDEV_BT:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = va->plus;
            break;

        case BFDEV_LT:
            retval = mpi_sub(dest, vb, va);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = vb->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_addi(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
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
        case BFDEV_EQ: default:
            retval = mpi_seti(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case BFDEV_LT:
            value = mpi_val(va);
            retval = mpi_seti(dest, vi - *value);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case BFDEV_BT:
            retval = mpi_subi(dest, va, vi);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = false;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_sub(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int diff, retval;

    if (va->plus != vb->plus) {
        retval = mpi_add(dest, va, vb);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = va->plus;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmp(va, vb);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case BFDEV_BT:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = va->plus;
            break;

        case BFDEV_LT:
            retval = mpi_sub(dest, vb, va);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = !vb->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_subi(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
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

    diff = mpi_cmpi(va, vi);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case BFDEV_LT:
            value = mpi_val(va);
            retval = mpi_seti(dest, vi - *value);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = false;
            break;

        case BFDEV_BT:
            retval = mpi_subi(dest, va, vi);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_mul(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    unsigned long cnta, cntb;
    int retval;

    cnta = mpi_len(va);
    cntb = mpi_len(vb);

    /* satisfy commutative law */
    if (cnta < cntb)
        bfdev_swap(va, vb);

    retval = mpi_mul(dest, va, vb);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = !(va->plus ^ vb->plus);

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_muli(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    int retval;

    retval = mpi_muli(dest, va, vi);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = va->plus;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_div(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int retval, diff;

    diff = mpi_cmp(va, vb);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(quot, 1);
            if (bfdev_unlikely(retval))
                return retval;

            retval = mpi_seti(rem, 0);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = !(va->plus ^ vb->plus);
            rem->plus = true;
            break;

        case BFDEV_LT:
            if (rem != va) {
                retval = mpi_set(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }

            /* quot can be equal to va */
            retval = mpi_seti(quot, 0);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = true;
            break;

        case BFDEV_BT:
            retval = mpi_div(quot, rem, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = !(va->plus ^ vb->plus);
            rem->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_divi(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    int retval, diff;

    diff = mpi_cmpi(va, vi);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(quot, 1);
            if (bfdev_unlikely(retval))
                return retval;

            retval = mpi_seti(rem, 0);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = va->plus;
            rem->plus = true;
            break;

        case BFDEV_LT:
            if (rem != va) {
                retval = mpi_set(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }

            /* quot can be equal to va */
            retval = mpi_seti(quot, 0);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = true;
            break;

        case BFDEV_BT:
            retval = mpi_divi(quot, rem, va, vi);
            if (bfdev_unlikely(retval))
                return retval;

            quot->plus = va->plus;
            rem->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_mod(bfdev_mpi_t *rem,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int retval, diff;

    diff = mpi_cmp(va, vb);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(rem, 0);
            if (bfdev_unlikely(retval))
                return retval;

            rem->plus = true;
            break;

        case BFDEV_LT:
            if (rem != va) {
                retval = mpi_set(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }
            break;

        case BFDEV_BT:
            retval = mpi_mod(rem, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            rem->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_modi(bfdev_mpi_t *rem,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    int retval, diff;

    diff = mpi_cmpi(va, vi);
    switch (diff) {
        case BFDEV_EQ: default:
            retval = mpi_seti(rem, 0);
            if (bfdev_unlikely(retval))
                return retval;

            rem->plus = true;
            break;

        case BFDEV_LT:
            if (rem != va) {
                retval = mpi_set(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }
            break;

        case BFDEV_BT:
            retval = mpi_modi(rem, va, vi);
            if (bfdev_unlikely(retval))
                return retval;

            rem->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_and(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int retval;

    retval = mpi_and(dest, va, vb);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = true;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_or(bfdev_mpi_t *dest,
             const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int retval;

    retval = mpi_or(dest, va, vb);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = true;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_xor(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int retval;

    retval = mpi_xor(dest, va, vb);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = true;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_shli(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift)
{
    int retval;

    retval = mpi_shli(dest, va, shift);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = va->plus;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_shri(bfdev_mpi_t *dest,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE shift)
{
    int retval;

    retval = mpi_shri(dest, va, shift);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = va->plus;

    return -BFDEV_ENOERR;
}

export bool
bfdev_mpi_btesti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit)
{
    BFDEV_MPI_TYPE *base;
    unsigned long offset, length;

    offset = BFDEV_BITS_WORD(bit);
    length = mpi_len(dest);

    if (offset >= length)
        return false;

    base = mpi_val(dest);
    return bfdev_bit_test(base, bit);
}

export int
bfdev_mpi_bseti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit)
{
    BFDEV_MPI_TYPE *base;
    unsigned long offset;
    int retval;

    offset = BFDEV_BITS_WORD(bit);
    retval = mpi_extend(dest, offset + 1);
    if (bfdev_unlikely(retval))
        return retval;

    base = mpi_val(dest);
    bfdev_bit_set(base, bit);

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_bclri(bfdev_mpi_t *dest, BFDEV_MPI_TYPE bit)
{
    BFDEV_MPI_TYPE *base;
    unsigned long offset, length;

    offset = BFDEV_BITS_WORD(bit);
    length = mpi_len(dest);

    if (offset >= length)
        return -BFDEV_ENOERR;

    base = mpi_val(dest);
    bfdev_bit_clr(base, bit);
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_seti(bfdev_mpi_t *dest, BFDEV_MPI_TYPE val)
{
    int retval;

    retval = mpi_seti(dest, val);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = true;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_set(bfdev_mpi_t *dest, const bfdev_mpi_t *src)
{
    int retval;

    retval = mpi_set(dest, src);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = src->plus;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_import(bfdev_mpi_t *var, const BFDEV_MPI_TYPE *buffer,
                 unsigned long length, bool sign)
{
    int retval;

    retval = mpi_copy(var, buffer, length);
    if (bfdev_unlikely(retval))
        return retval;

    var->plus = sign;

    return -BFDEV_ENOERR;
}

export const BFDEV_MPI_TYPE *
bfdev_mpi_data(const bfdev_mpi_t *var, bool *sign)
{
    const BFDEV_MPI_TYPE *retval;

    if (!mpi_len(var))
        retval = &mpi_zero;
    else
        retval = mpi_val(var);

    if (sign)
        *sign = var->plus;

    return retval;
}

export void
bfdev_mpi_release(bfdev_mpi_t *var)
{
    bfdev_array_release(&var->value);
}
