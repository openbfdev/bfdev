/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log2.h>
#include <bfdev/ringbuf.h>
#include <bfdev/bits.h>
#include <export.h>

#define RINGBUF_GENERIC_COPY(copy1, copy2, fold1, fold2) do {   \
    unsigned long size = ringbuf->mask + 1;                     \
    unsigned long esize = ringbuf->esize;                       \
    unsigned long llen;                                         \
                                                                \
    offset &= ringbuf->mask;                                    \
    if (esize != 1) {                                           \
        offset *= esize;                                        \
        size *= esize;                                          \
        len *= esize;                                           \
    }                                                           \
                                                                \
    llen = bfdev_min(len, size - offset);                       \
    memcpy(copy1, copy2, llen);                                 \
    memcpy(fold1, fold2, len - llen);                           \
} while (0)

static __bfdev_always_inline void
ringbuf_out_copy(struct bfdev_ringbuf *ringbuf, void *buff,
                 unsigned long len, unsigned long offset)
{
    RINGBUF_GENERIC_COPY(
        buff, ringbuf->data + offset,
        buff + llen, ringbuf->data
    );
}

static __bfdev_always_inline void
ringbuf_in_copy(struct bfdev_ringbuf *ringbuf, const void *buff,
                unsigned long len, unsigned long offset)
{
    RINGBUF_GENERIC_COPY(
        ringbuf->data + offset, buff,
        ringbuf->data, buff + llen
    );
}

static __bfdev_always_inline unsigned long
ringbuf_record_peek(struct bfdev_ringbuf *ringbuf, unsigned long recsize)
{
    unsigned long mask = ringbuf->mask;
    unsigned long offset = ringbuf->out;
    unsigned long length = 0;
    uint8_t *data = ringbuf->data;

    if (ringbuf->esize != 1) {
        offset *= ringbuf->esize;
        mask *= ringbuf->esize;
        mask += ringbuf->esize - 1;
    }

    while (recsize--) {
        length <<= BFDEV_BITS_PER_U8;
        length |= data[offset & mask];
        offset += ringbuf->esize;
    }

    return length;
}

static __bfdev_always_inline void
ringbuf_record_poke(struct bfdev_ringbuf *ringbuf, unsigned long len,
                    unsigned long recsize)
{
    unsigned long mask = ringbuf->mask;
    unsigned long offset = ringbuf->out;
    uint8_t *data = ringbuf->data;

    if (ringbuf->esize != 1) {
        offset *= ringbuf->esize;
        mask *= ringbuf->esize;
        mask += ringbuf->esize - 1;
    }

    while (recsize--) {
        data[offset & mask] = (uint8_t)len;
        offset += ringbuf->esize;
        len >>= BFDEV_BITS_PER_U8;
    }
}

static inline bool
ringbuf_empty(struct bfdev_ringbuf *ringbuf)
{
    return ringbuf->in == ringbuf->out;
}

static inline unsigned long
ringbuf_valid(struct bfdev_ringbuf *ringbuf)
{
    return ringbuf->in - ringbuf->out;
}

static inline unsigned long
ringbuf_overflow(struct bfdev_ringbuf *ringbuf)
{
    unsigned long size = ringbuf->mask + 1;
    unsigned long used;

    used = ringbuf->in - ringbuf->out;
    if (used > size)
        return used - size;

    return 0;
}

export unsigned long
bfdev_ringbuf_peek_flat(struct bfdev_ringbuf *ringbuf, void *buff,
                        unsigned long len)
{
    unsigned long valid;

    valid = ringbuf_valid(ringbuf);
    bfdev_min_adj(len, valid);
    ringbuf_out_copy(ringbuf, buff, len, ringbuf->out);

    return len;
}

export unsigned long
bfdev_ringbuf_out_flat(struct bfdev_ringbuf *ringbuf, void *buff,
                       unsigned long len)
{
    unsigned long llen;

    llen = bfdev_ringbuf_peek_flat(ringbuf, buff, len);
    ringbuf->out += llen;

    return llen;
}

export unsigned long
bfdev_ringbuf_in_flat(struct bfdev_ringbuf *ringbuf, const void *buff,
                      unsigned long len)
{
    unsigned long size = ringbuf->mask + 1;
    unsigned long overflow;

    bfdev_min_adj(len, size);
    ringbuf_in_copy(ringbuf, buff, len, ringbuf->in);
    ringbuf->in += len;

    overflow = ringbuf_overflow(ringbuf);
    if (overflow)
        ringbuf->out += overflow;

    return len;
}

export unsigned long
bfdev_ringbuf_peek_record(struct bfdev_ringbuf *ringbuf, void *buff,
                          unsigned long len, unsigned long record)
{
    unsigned long datalen;

    if (ringbuf_empty(ringbuf))
        return 0;

    datalen = ringbuf_record_peek(ringbuf, record);
    bfdev_min_adj(len, datalen);
    ringbuf_out_copy(ringbuf, buff, len, ringbuf->out + record);

    return len;
}

export unsigned long
bfdev_ringbuf_out_record(struct bfdev_ringbuf *ringbuf, void *buff,
                         unsigned long len, unsigned long record)
{
    unsigned long datalen;

    if (ringbuf_empty(ringbuf))
        return 0;

    datalen = ringbuf_record_peek(ringbuf, record);
    bfdev_min_adj(len, datalen);
    ringbuf_out_copy(ringbuf, buff, len, ringbuf->out + record);
	ringbuf->out += datalen + record;

    return len;
}

export unsigned long
bfdev_ringbuf_in_record(struct bfdev_ringbuf *ringbuf, const void *buff,
                        unsigned long len, unsigned long record)
{
    unsigned long size = ringbuf->mask + 1;
    unsigned long offset, overflow, datalen;

    bfdev_min_adj(len, size);
    offset = ringbuf->in + record;
    ringbuf->in = offset + len;

    overflow = ringbuf_overflow(ringbuf);
    while (overflow) {
        datalen = record + ringbuf_record_peek(ringbuf, record);
	    ringbuf->out += datalen;
        overflow -= bfdev_min(datalen, overflow);
    }

    ringbuf_record_poke(ringbuf, len, record);
    ringbuf_in_copy(ringbuf, buff, len, offset);

    return len;
}

export int
bfdev_ringbuf_dynamic_alloc(struct bfdev_ringbuf *ringbuf, const struct bfdev_alloc *alloc,
                            size_t esize, size_t size)
{
    size = bfdev_pow2_roundup(size);
    if (size < 2)
        return -BFDEV_EINVAL;

    ringbuf->data = bfdev_malloc_array(alloc, size, esize);
    if (!ringbuf->data)
        return -BFDEV_ENOMEM;

    ringbuf->in = 0;
    ringbuf->out = 0;
    ringbuf->mask = size - 1;
    ringbuf->esize = esize;
    ringbuf->alloc = alloc;

    return -BFDEV_ENOERR;
}

export void
bfdev_ringbuf_dynamic_free(struct bfdev_ringbuf *ringbuf)
{
    const struct bfdev_alloc *alloc = ringbuf->alloc;

    ringbuf->in = 0;
    ringbuf->out = 0;
    ringbuf->mask = 0;
    ringbuf->esize = 0;

    bfdev_free(alloc, ringbuf->data);
    ringbuf->data = NULL;
}
