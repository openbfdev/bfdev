/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/mfp.h>
#include <bfdev/minmax.h>
#include <bfdev/array.h>
#include <export.h>

export const struct bfdev_mfp
bfdev_mfp_zero = {
    .length = 1,
    .plus = true,
    .block = {0},
};

export const struct bfdev_mfp
bfdev_mfp_one = {
    .length = 1,
    .plus = true,
    .block = {1},
};

static inline void
relocation_value(struct bfdev_mfp *var)
{
    while (!var->value && var->length) {
        var->value++;
        var->length--;
    }
}

static int
mfp_compare(const struct bfdev_mfp *va, const struct bfdev_mfp *vb,
            bool unsign)
{
    BFDEV_MFP_TYPE *ptra, *ptrb;
    unsigned int count;

    if (!unsign && va->plus != vb->plus) {
        if (va->plus)
            return 1;
        else
            return -1;
    }

    if (va->length != vb->length) {
        if (va->length > vb->length) {
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

    count = va->length + bfdev_min(va->scale, vb->length);
    ptra = va->value;
    ptrb = vb->value;

    while (count && *ptra == *ptrb) {
        count--;
        ptra++;
        ptrb++;
    }

    if (count) {
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

    if (va->scale != vb->scale) {
        if (va->scale > vb->scale) {
            for (count = va->scale - vb->scale; count; --count) {
                if (!*ptra++)
                    continue;

                if (unsign || va->plus)
                    return 1;
                else
                    return -1;
            }
        } else {
            for (count = vb->scale - va->scale; count; --count) {
                if (!*ptrb++)
                    continue;

                if (unsign || va->plus)
                    return 1;
                else
                    return -1;
            }
        }
    }

    return 0;
}

static struct bfdev_mfp *
mfp_add(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
        const struct bfdev_mfp *vb)
{
    BFDEV_MFP_TYPE *ptra, *ptrb, *ptrs;
    unsigned int length, scale, carry;
    unsigned int cnta, cntb;
    struct bfdev_mfp *result;

    length = bfdev_max(va->length, vb->length);
    scale = bfdev_max(va->scale, vb->scale);

    result = bfdev_mfp_create(alloc, length + 1, scale);
    if (bfdev_unlikely(!result))
        return NULL;

    cnta = va->scale;
    cntb = vb->scale;

    ptra = va->value + (va->length + cnta - 1);
    ptrb = vb->value + (vb->length + cntb - 1);
    ptrs = result->value + (length + scale - 1);

    if (cnta != cntb) {
	    while (cnta > cntb) {
            *ptrs-- = *ptra--;
            cnta--;
        }
	    while (cnta < cntb) {
            *ptrs-- = *ptrb--;
            cntb--;
        }
    }

    cnta += va->length;
    cntb += vb->length;
    carry = 0;

    while (cnta && cntb) {
        *ptrs = *ptra-- + *ptrb-- + carry;

        if (*ptrs < BFDEV_MFP_BASE)
            carry = 0;
        else {
            carry = 1;
            *ptrs -= BFDEV_MFP_BASE;
        }

        ptrs--;
        cnta--;
        cntb--;
    }

    if (!cnta) {
        cnta = cntb;
        ptra = ptrb;
    }

    while (cnta) {
        *ptrs = *ptra-- + carry;

        if (*ptrs < BFDEV_MFP_BASE)
            carry = 0;
        else {
            carry = 1;
            *ptrs -= BFDEV_MFP_BASE;
        }

        ptrs--;
        cnta--;
    }

    if (carry == 1)
        *ptrs += 1;

    relocation_value(result);
    return result;
}

static struct bfdev_mfp *
mfp_sub(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
        const struct bfdev_mfp *vb)
{
    BFDEV_MFP_TYPE *ptra, *ptrb, *ptrs, value;
    unsigned int length, scale, borrow;
    unsigned int dlength, dscale, count;
    struct bfdev_mfp *result;

    dlength = bfdev_max(va->length, vb->length);
    dscale = bfdev_max(va->scale, vb->scale);
    length = bfdev_min(va->length, vb->length);
    scale = bfdev_min(va->scale, vb->scale);

    result = bfdev_mfp_create(alloc, dlength, dscale);
    if (bfdev_unlikely(!result))
        return NULL;

    ptra = va->value + (va->length + va->scale - 1);
    ptrb = vb->value + (vb->length + vb->scale - 1);
    ptrs = result->value + (length + scale - 1);
    borrow = 0;

    if (va->scale != scale) {
        for (count = va->scale - scale; count; --count)
	        *ptrs-- = *ptra--;
    } else {
        for (count = vb->scale - scale; count; --count) {
            value = - *ptrb-- - borrow;

            if (value < BFDEV_MFP_BASE)
                borrow = 0;
            else {
                borrow = 1;
                value += BFDEV_MFP_BASE;
            }

            *ptrs-- = value;
        }
    }

    for (count = length + scale; count; --count) {
        value = *ptra-- - *ptrb-- - borrow;

        if (value < BFDEV_MFP_BASE)
            borrow = 0;
        else {
            borrow = 1;
            value += BFDEV_MFP_BASE;
        }

        *ptrs-- = value;
    }

    if (dlength != length) {
        for (count = dlength - length; count; --count) {
            value = *ptra-- - borrow;

            if (value < BFDEV_MFP_BASE)
                borrow = 0;
            else {
                borrow = 1;
                value += BFDEV_MFP_BASE;
            }

            *ptrs-- = value;
        }
    }

    relocation_value(result);
    return result;
}

export const struct bfdev_mfp *
bfdev_mfp_add(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
              const struct bfdev_mfp *vb)
{
    struct bfdev_mfp *result;
    int value;

    if (va->plus == vb->plus) {
        result = mfp_add(alloc, va, vb);
        if (bfdev_unlikely(!result))
            return NULL;

        result->plus = va->plus;
        return result;
    }

    value = mfp_compare(va, vb, true);
    switch (value) {
        case 0: default:
            return &bfdev_mfp_zero;

        case 1:
            result = mfp_sub(alloc, va, vb);
            result->plus = va->plus;
            break;

        case -1:
            result = mfp_sub(alloc, vb, va);
            result->plus = vb->plus;
            break;
    }

    return result;
}

export const struct bfdev_mfp *
bfdev_mfp_sub(const struct bfdev_alloc *alloc, const struct bfdev_mfp *va,
              const struct bfdev_mfp *vb)
{
    struct bfdev_mfp *result;
    int value;

    if (va->plus != vb->plus) {
        result = mfp_add(alloc, va, vb);
        if (bfdev_unlikely(!result))
            return NULL;

        result->plus = va->plus;
        return result;
    }

    value = mfp_compare(va, vb, true);
    switch (value) {
        case 0: default:
            return &bfdev_mfp_zero;

        case 1:
            result = mfp_sub(alloc, va, vb);
            result->plus = va->plus;
            break;

        case -1:
            result = mfp_sub(alloc, vb, va);
            result->plus = !vb->plus;
            break;
    }

    return result;
}

export struct bfdev_mfp *
bfdev_mfp_create(const struct bfdev_alloc *alloc, unsigned int length,
                 unsigned int scale)
{
    struct bfdev_mfp *result;

    result = bfdev_zalloc(alloc, sizeof(*result) +
                          sizeof(*result->block) * (length + scale));
    if (bfdev_unlikely(!result))
        return NULL;

    result->length = length;
    result->scale = scale;
    result->plus = true;
    result->value = result->block;

    return result;
}

export void
bfdev_mfp_destory(const struct bfdev_alloc *alloc, const struct bfdev_mfp *mfp)
{
    bfdev_free(alloc, mfp);
}
