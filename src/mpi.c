/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/mpi.h>
#include <bfdev/dword.h>
#include <export.h>

#define mpi_len(mpi) bfdev_array_index(&(mpi)->value)
#define mpi_val(mpi) bfdev_array_data(&(mpi)->value, 0)

/**
 * Multi Precision Array Computing.
 *
 * The following functions are used to
 * calculating raw data within an array.
 */

static inline void
mpa_zero(BFDEV_MPI_TYPE *dest, unsigned long length)
{
    memset(dest, 0, length * sizeof(*dest));
}

static inline void
mpa_copy(BFDEV_MPI_TYPE *dest, const BFDEV_MPI_TYPE *src,
         unsigned long length)
{
    memcpy(dest, src, length * sizeof(*dest));
}

static inline int
mpa_cmpi(const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length)
{
    if (length > 1)
        return 1;

    if (*ptra != vi) {
        if (*ptra > vi)
            return 1;
        return -1;
    }

    return 0;
}

static inline int
mpa_cmp(const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
        unsigned long lena, unsigned long lenb)
{
    unsigned long length;

    if (lena != lenb) {
        if (lena > lenb)
            return 1;
        return -1;
    }

    ptra += lena - 1;
    ptrb += lenb - 1;

    for (length = lena; length; --length) {
        if (*ptra != *ptrb)
            break;
        ptra--;
        ptrb--;
    }

    if (length) {
        if (*ptra > *ptrb)
            return 1;
        return -1;
    }

    return 0;
}

static inline bool
mpa_addi(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
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
mpa_add(BFDEV_MPI_TYPE *ptrs,
        const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
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
mpa_subi(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
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
mpa_sub(BFDEV_MPI_TYPE *ptrs,
        const BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
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
mpa_muli(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
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
mpa_maci(BFDEV_MPI_TYPE *ptrs,
         const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length, BFDEV_MPI_TYPE carry)
{
    BFDEV_MPI_TYPE vhigh, vlow;

    while (length--) {
        bfdev_dword_umul(vhigh, vlow, *ptra++, vi);
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
        bfdev_dword_umul(vhigh, vlow, *ptra++, vi);
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
    BFDEV_MPI_TYPE value, rem;
    unsigned long index;

    index = length - 1;
    ptrs += index;
    ptra += index;
    rem = *ptra;

    if (rem >= vi)
        rem = 0;
    else {
        *ptrs-- = 0;
        ptra--;
        length--;
    }

    while (length--) {
        value = *ptra--;
        bfdev_dword_udiv(*ptrs--, rem, rem, value, vi);
    }

    return rem;
}

static inline BFDEV_MPI_TYPE
mpa_modi(const BFDEV_MPI_TYPE *ptra, BFDEV_MPI_TYPE vi,
         unsigned long length)
{
    BFDEV_MPI_TYPE dummy __bfdev_maybe_unused;
    BFDEV_MPI_TYPE value, rem;
    unsigned long index;

    index = length - 1;
    ptra += index;
    rem = *ptra;

    if (rem >= vi)
        rem = 0;
    else {
        ptra--;
        length--;
    }

    while (length--) {
        value = *ptra--;
        bfdev_dword_udiv(dummy, rem, rem, value, vi);
    }

    return rem;
}

static inline bool
mpa_divrem(BFDEV_MPI_TYPE *ptrs,
           BFDEV_MPI_TYPE *ptra, const BFDEV_MPI_TYPE *ptrb,
           unsigned long cnta, unsigned long cntb)
{
    BFDEV_MPI_TYPE dhigh, dlow, value;
    unsigned long index;
    bool limb;

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

    dlow = ptra[cntb - 2];
    dhigh = ptra[cntb - 1];
    value = ptrb[cntb - 1];

    limb = false;
    if (value >= dhigh) {
        if (value > dhigh || mpa_cmp(ptra, ptrb, cntb - 1, cntb - 1) >= 0) {
            mpa_sub(ptra, ptra, ptrb, cntb, cntb, false);
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
            BFDEV_MPI_TYPE rem;

            bfdev_dword_udiv(quot, rem, value, ptra[cntb - 1], dhigh);
            bfdev_dword_umul(v1, value, dlow, quot);

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
mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *value;
    unsigned long length;

    length = mpi_len(va);
    value = mpi_val(va);

    return mpa_cmpi(value, vi, length);
}

static inline int
mpi_cmp(const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptra, *ptrb;
    unsigned long lena, lenb;

    lena = mpi_len(va);
    lenb = mpi_len(vb);

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

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length + 1);
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

    cntb = mpi_len(vb);
    ptrb = mpi_val(vb);

    if (cntb == 1)
        return mpi_addi(dest, va, *ptrb);

    cnta = mpi_len(va);
    length = bfdev_max(cnta, cntb);

    retval = bfdev_array_resize(&dest->value, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

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

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    borrow = mpa_subi(ptrs, ptra, vi, length, false);
    if (bfdev_unlikely(borrow))
        return -BFDEV_EOVERFLOW;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_sub(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bool borrow;
    int retval;

    cntb = mpi_len(vb);
    ptrb = mpi_val(vb);

    if (cntb == 1)
        return mpi_subi(dest, va, *ptrb);

    cnta = mpi_len(va);
    length = bfdev_max(cnta, cntb);

    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    borrow = mpa_sub(ptrs, ptra, ptrb, cnta, cntb, false);
    if (bfdev_unlikely(borrow))
        return -BFDEV_EOVERFLOW;
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

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length + 1);
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
mpi_maci(bfdev_mpi_t *dest,
         const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    BFDEV_MPI_TYPE carry;
    unsigned long length;
    int retval;

    length = mpi_len(va);

    retval = bfdev_array_resize(&dest->value, length + 1);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(va);

    carry = mpa_maci(ptrs, ptra, vi, length, 0);
    *(ptrs + length) = carry;
    mpi_relocation(dest);

    return -BFDEV_ENOERR;
}

static inline int
mpi_mul(bfdev_mpi_t *dest,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long length, cnta, cntb;
    bfdev_array_t *buffer, array;
    int retval;
    bool nval;

    cntb = mpi_len(vb);
    ptrb = mpi_val(vb);

    if (cntb == 1)
        return mpi_muli(dest, va, *ptrb);

    nval = false;
    if (dest != va && dest != vb)
        buffer = &dest->value;
    else {
        bfdev_array_init(&array, dest->value.alloc, sizeof(*ptrs));
        buffer = &array;
        nval = true;
    }

    cnta = mpi_len(va);
    length = cnta + cntb;

    retval = bfdev_array_resize(buffer, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = bfdev_array_data(buffer, 0);
    ptra = mpi_val(va);

    mpa_mul(ptrs, ptra, ptrb, cnta, cntb);
    if (nval) {
        bfdev_array_release(&dest->value);
        dest->value = array;
    }
    mpi_relocation(dest);

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

    length = mpi_len(va);

    if (quot) {
        retval = bfdev_array_resize(&quot->value, length);
        if (bfdev_unlikely(retval))
            return retval;

        ptrs = mpi_val(quot);
    }

    ptra = mpi_val(va);

    if (!quot)
        value = mpa_modi(ptra, vi, length);
    else
        value = mpa_divmodi(ptrs, ptra, vi, length);

    if (rem)
        mpi_set(rem, value);

    if (quot)
        mpi_relocation(quot);

    return -BFDEV_ENOERR;
}

static inline int
mpi_div(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
        const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    BFDEV_MPI_TYPE *ptrs, *ptra, *ptrb;
    unsigned long cnta, cntb, length;
    bool limb;
    int retval;

    cntb = mpi_len(vb);
    ptrb = mpi_val(vb);

    if (cntb == 1)
        return mpi_divi(quot, rem, va, *ptrb);

    /* divide by zero */
    if (bfdev_unlikely(!mpi_cmpi(vb, 0)))
        return -BFDEV_EOVERFLOW;

    cnta = mpi_len(va);
    length = cnta - cntb;






    if (quot) {
        retval = bfdev_array_resize(&quot->value, length);
        if (bfdev_unlikely(retval))
            return retval;
    }

    retval = bfdev_mpi_copy(rem, va);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(quot);
    ptra = mpi_val(rem);

    limb = mpa_divrem(ptrs, ptra, ptrb, cnta, cntb);
	if (rem) {
        retval = bfdev_array_resize(&rem->value, cntb);
        if (bfdev_unlikely(retval))
            return retval;

        mpi_relocation(rem);
    }

	if (quot) {
		length = cnta - cntb;
		if (limb) {
			ptrs[length] = limb;
			length += 1;
		}

        mpi_relocation(quot);
    }

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

    if (va->plus != vb->plus) {
        if (va->plus)
            return 1;
        return -1;
    }

    diff = mpi_cmp(va, vb);
    if (!diff)
        return 0;

    if (diff > 0) {
        if (va->plus)
            return 1;
        return -1;
    }

    /* diff < 0 */
    if (va->plus)
        return -1;
    return 1;
}

export int
bfdev_mpi_cmpi(const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    int diff;

    if (!va->plus)
        return -1;

    diff = mpi_cmpi(va, vi);
    if (!diff)
        return 0;

    if (diff > 0) {
        if (va->plus)
            return 1;
        return -1;
    }

    /* diff < 0 */
    if (va->plus)
        return -1;
    return 1;
}

export int
bfdev_mpi_add(bfdev_mpi_t *dest,
              const bfdev_mpi_t *va, const bfdev_mpi_t *vb)
{
    int diff, retval;

    if (va->plus == vb->plus) {
        retval = mpi_add(dest, va, vb);
        if (bfdev_unlikely(retval))
            return retval;

        dest->plus = va->plus;
        return -BFDEV_ENOERR;
    }

    diff = mpi_cmp(va, vb);
    switch (diff) {
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case 1:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = va->plus;
            break;

        case -1:
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
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case -1:
            value = mpi_val(va);
            retval = mpi_set(dest, vi - *value);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case 1:
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
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case 1:
            retval = mpi_sub(dest, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = va->plus;
            break;

        case -1:
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
        case 0: default:
            retval = mpi_set(dest, 0);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = true;
            break;

        case -1:
            value = mpi_val(va);
            retval = mpi_set(dest, vi - *value);
            if (bfdev_unlikely(retval))
                return retval;

            dest->plus = false;
            break;

        case 1:
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

    if (cnta < cntb) {
        bfdev_swap(va, vb);
        bfdev_swap(cnta, cntb);
    }

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
        case 0: default:
            if (quot) {
                retval = mpi_set(quot, 1);
                if (bfdev_unlikely(retval))
                    return retval;

                quot->plus = !(va->plus ^ vb->plus);
            }

            if (rem) {
                retval = mpi_set(rem, 0);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = true;
            }
            break;

        case -1:
            if (quot) {
                retval = mpi_set(quot, 0);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = true;
            }

            if (rem) {
                retval = bfdev_mpi_copy(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }
            break;

        case 1:
            retval = mpi_div(quot, rem, va, vb);
            if (bfdev_unlikely(retval))
                return retval;

            if (quot)
                quot->plus = !(va->plus ^ vb->plus);

            if (rem)
                rem->plus = va->plus;
            break;
    }

    return retval;
}

export int
bfdev_mpi_divi(bfdev_mpi_t *quot, bfdev_mpi_t *rem,
               const bfdev_mpi_t *va, BFDEV_MPI_TYPE vi)
{
    int retval, diff;

    diff = mpi_cmpi(va, vi);
    switch (diff) {
        case 0: default:
            if (quot) {
                retval = mpi_set(quot, 1);
                if (bfdev_unlikely(retval))
                    return retval;

                quot->plus = va->plus;
            }

            if (rem) {
                retval = mpi_set(rem, 0);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = true;
            }
            break;

        case -1:
            if (quot) {
                retval = mpi_set(quot, 0);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = true;
            }

            if (rem) {
                retval = bfdev_mpi_copy(rem, va);
                if (bfdev_unlikely(retval))
                    return retval;

                rem->plus = va->plus;
            }
            break;

        case 1:
            retval = mpi_divi(quot, rem, va, vi);
            if (bfdev_unlikely(retval))
                return retval;

            if (quot)
                quot->plus = va->plus;
            break;
    }

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_set(bfdev_mpi_t *dest, BFDEV_MPI_TYPE val)
{
    int retval;

    retval = mpi_set(dest, val);
    if (bfdev_unlikely(retval))
        return retval;

    dest->plus = true;

    return -BFDEV_ENOERR;
}

export int
bfdev_mpi_copy(bfdev_mpi_t *dest, const bfdev_mpi_t *src)
{
    BFDEV_MPI_TYPE *ptrs, *ptra;
    unsigned long length;
    int retval;

    if (dest == src)
        return -BFDEV_EINVAL;

    length = mpi_len(src);
    retval = bfdev_array_resize(&dest->value, length);
    if (bfdev_unlikely(retval))
        return retval;

    ptrs = mpi_val(dest);
    ptra = mpi_val(src);

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
