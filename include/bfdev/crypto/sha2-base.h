/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_SHA2_BASE_H_
#define _BFDEV_CRYPTO_SHA2_BASE_H_

#include <bfdev/config.h>
#include <bfdev/string.h>
#include <bfdev/byteorder.h>
#include <bfdev/unaligned.h>
#include <bfdev/sha2.h>

BFDEV_BEGIN_DECLS

typedef void (*bfdev_sha2_bfn_t)
(bfdev_sha2_ctx_t *ctx, const void *src, size_t block);

static inline void
bfdev_sha2_base_update(bfdev_sha2_ctx_t *ctx, const void *data,
                       size_t size, bfdev_sha2_bfn_t func)
{
    unsigned int partial;
    size_t length, blocks;

    partial = ctx->count % BFDEV_SHA2_BLOCK_SIZE;
    ctx->count += size;

    if (bfdev_unlikely((partial + size) >= BFDEV_SHA2_BLOCK_SIZE)) {
        if (partial) {
            length = BFDEV_SHA2_BLOCK_SIZE - partial;
            bfport_memcpy(ctx->buffer + partial, data, length);
            func(ctx, ctx->buffer, 1);

            data += length;
            size -= length;
        }

        blocks = size / BFDEV_SHA2_BLOCK_SIZE;
        size %= BFDEV_SHA2_BLOCK_SIZE;

        if (blocks) {
            func(ctx, data, blocks);
            data += blocks * BFDEV_SHA2_BLOCK_SIZE;
        }

        partial = 0;
    }

    if (size)
        bfport_memcpy(ctx->buffer + partial, data, size);
}

static inline void
bfdev_sha2_base_finalize(bfdev_sha2_ctx_t *ctx, bfdev_sha2_bfn_t func)
{
    unsigned int offset, partial;
    bfdev_be64 *plen;

    offset = BFDEV_SHA2_BLOCK_SIZE - BFDEV_BYTES_PER_U64;
    plen = (bfdev_be64 *)(ctx->buffer + offset);

    partial = ctx->count % BFDEV_SHA2_BLOCK_SIZE;
    ctx->buffer[partial++] = 0x80;

    if (offset < partial) {
        bfport_memset(ctx->buffer + partial, 0, BFDEV_SHA2_BLOCK_SIZE - partial);
        func(ctx, ctx->buffer, 1);
        partial = 0;
    }

    bfport_memset(ctx->buffer + partial, 0, offset - partial);
    bfdev_unaligned_set_be64(plen, (uint64_t)ctx->count << 3);
    func(ctx, ctx->buffer, 1);
}

static inline void
bfdev_sha2_base_finish(bfdev_sha2_ctx_t *ctx, unsigned int dsize, void *buff)
{
    bfdev_be32 *digest;
    unsigned int count;

    digest = (bfdev_be32 *)buff;
    for (count = 0; count < dsize; ++count)
        bfdev_unaligned_set_be32(digest++, ctx->state[count]);
}

static inline void
bfdev_sha224_digest_init(uint32_t *digest)
{
    digest[0] = BFDEV_SHA224_DIGEST0;
    digest[1] = BFDEV_SHA224_DIGEST1;
    digest[2] = BFDEV_SHA224_DIGEST2;
    digest[3] = BFDEV_SHA224_DIGEST3;
    digest[4] = BFDEV_SHA224_DIGEST4;
    digest[5] = BFDEV_SHA224_DIGEST5;
    digest[6] = BFDEV_SHA224_DIGEST6;
    digest[7] = BFDEV_SHA224_DIGEST7;
}

static inline void
bfdev_sha256_digest_init(uint32_t *digest)
{
    digest[0] = BFDEV_SHA256_DIGEST0;
    digest[1] = BFDEV_SHA256_DIGEST1;
    digest[2] = BFDEV_SHA256_DIGEST2;
    digest[3] = BFDEV_SHA256_DIGEST3;
    digest[4] = BFDEV_SHA256_DIGEST4;
    digest[5] = BFDEV_SHA256_DIGEST5;
    digest[6] = BFDEV_SHA256_DIGEST6;
    digest[7] = BFDEV_SHA256_DIGEST7;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_SHA2_BASE_H_ */
