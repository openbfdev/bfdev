/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log2.h>
#include <bfdev/fifo.h>
#include <bfdev/bits.h>
#include <export.h>

#define FIFO_GENERIC_COPY(copy1, copy2, fold1, fold2) do {  \
    unsigned long size, esize, llen;                        \
                                                            \
    size = fifo->mask + 1;                                  \
    esize = fifo->esize;                                    \
    offset &= fifo->mask;                                   \
                                                            \
    if (esize != 1) {                                       \
        offset *= esize;                                    \
        size *= esize;                                      \
        len *= esize;                                       \
    }                                                       \
                                                            \
    llen = bfdev_min(len, size - offset);                   \
    memcpy(copy1, copy2, llen);                             \
    memcpy(fold1, fold2, len - llen);                       \
} while (0)

static __bfdev_always_inline void
fifo_out_copy(struct bfdev_fifo *fifo, void *buff, unsigned long len, unsigned long offset)
{
    FIFO_GENERIC_COPY(
        buff, fifo->data + offset,
        buff + llen, fifo->data
    );
}

static __bfdev_always_inline void
fifo_in_copy(struct bfdev_fifo *fifo, const void *buff, unsigned long len, unsigned long offset)
{
    FIFO_GENERIC_COPY(
        fifo->data + offset, buff,
        fifo->data, buff + llen
    );
}

static __bfdev_always_inline unsigned long
fifo_record_peek(struct bfdev_fifo *fifo, unsigned long recsize)
{
    unsigned long mask, offset, length;
    uint8_t *data;

    mask = fifo->mask;
    offset = fifo->out;
    data = fifo->data;
    length = 0;

    if (fifo->esize != 1) {
        offset *= fifo->esize;
        mask *= fifo->esize;
        mask += fifo->esize - 1;
    }

    while (recsize--) {
        length <<= BFDEV_BITS_PER_U8;
        length |= data[offset & mask];
        offset += fifo->esize;
    }

    return length;
}

static __bfdev_always_inline void
fifo_record_poke(struct bfdev_fifo *fifo, unsigned long len,
                 unsigned long recsize)
{
    unsigned long mask, offset;
    uint8_t *data;

    mask = fifo->mask;
    offset = fifo->out;
    data = fifo->data;

    if (fifo->esize != 1) {
        offset *= fifo->esize;
        mask *= fifo->esize;
        mask += fifo->esize - 1;
    }

    while (recsize--) {
        data[offset & mask] = (uint8_t)len;
        offset += fifo->esize;
        len >>= BFDEV_BITS_PER_U8;
    }
}

static inline bool
fifo_empty(struct bfdev_fifo *fifo)
{
    return fifo->in == fifo->out;
}

static inline unsigned long
fifo_valid(struct bfdev_fifo *fifo)
{
    return fifo->in - fifo->out;
}

static inline unsigned long
fifo_unused(struct bfdev_fifo *fifo)
{
    return (fifo->mask + 1) - (fifo->in - fifo->out);
}

export unsigned long
bfdev_fifo_peek_flat(struct bfdev_fifo *fifo, void *buff,
                     unsigned long len)
{
    unsigned long valid;

    valid = fifo_valid(fifo);
    bfdev_min_adj(len, valid);
    fifo_out_copy(fifo, buff, len, fifo->out);

    return len;
}

export unsigned long
bfdev_fifo_out_flat(struct bfdev_fifo *fifo, void *buff,
                    unsigned long len)
{
    unsigned long llen;

    llen = bfdev_fifo_peek_flat(fifo, buff, len);
    fifo->out += llen;

    return llen;
}

export unsigned long
bfdev_fifo_in_flat(struct bfdev_fifo *fifo, const void *buff,
                   unsigned long len)
{
    unsigned long unused;

    unused = fifo_unused(fifo);
    bfdev_min_adj(len, unused);
    fifo_in_copy(fifo, buff, len, fifo->in);
    fifo->in += len;

    return len;
}

export unsigned long
bfdev_fifo_peek_record(struct bfdev_fifo *fifo, void *buff,
                       unsigned long len, unsigned long record)
{
    unsigned long datalen;

    if (fifo_empty(fifo))
        return 0;

    datalen = fifo_record_peek(fifo, record);
    bfdev_min_adj(len, datalen);
    fifo_out_copy(fifo, buff, len, fifo->out + record);

    return len;
}

export unsigned long
bfdev_fifo_out_record(struct bfdev_fifo *fifo, void *buff,
                      unsigned long len, unsigned long record)
{
    unsigned long datalen;

    if (fifo_empty(fifo))
        return 0;

    datalen = fifo_record_peek(fifo, record);
    bfdev_min_adj(len, datalen);
    fifo_out_copy(fifo, buff, len, fifo->out + record);
    fifo->out += datalen + record;

    return len;
}

export unsigned long
bfdev_fifo_in_record(struct bfdev_fifo *fifo, const void *buff,
                     unsigned long len, unsigned long record)
{
    if (len + record > fifo_unused(fifo))
        return 0;

    fifo_record_poke(fifo, len, record);
    fifo_in_copy(fifo, buff, len, fifo->in + record);
    fifo->in += len + record;

    return len;
}

export int
bfdev_fifo_dynamic_alloc(struct bfdev_fifo *fifo, const bfdev_alloc_t *alloc,
                         size_t esize, size_t size)
{
    size = bfdev_pow2_roundup(size);
    if (size < 2)
        return -BFDEV_EINVAL;

    fifo->data = bfdev_malloc_array(alloc, size, esize);
    if (!fifo->data)
        return -BFDEV_ENOMEM;

    fifo->in = 0;
    fifo->out = 0;
    fifo->mask = size - 1;
    fifo->esize = esize;
    fifo->alloc = alloc;

    return -BFDEV_ENOERR;
}

export void
bfdev_fifo_dynamic_free(struct bfdev_fifo *fifo)
{
    const bfdev_alloc_t *alloc;

    fifo->in = 0;
    fifo->out = 0;
    fifo->mask = 0;
    fifo->esize = 0;

    alloc = fifo->alloc;
    bfdev_free(alloc, fifo->data);
    fifo->data = NULL;
}
