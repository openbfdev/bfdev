/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/sha2.h>
#include <bfdev/crypto/sha2.h>
#include <bfdev/crypto/sha2-base.h>
#include <export.h>

static void
sha2_transform_block(bfdev_sha2_ctx_t *ctx, const void *src, size_t blocks)
{
    uint32_t buffer[BFDEV_SHA2_WORKSPACE_WORDS];

    while (blocks--) {
        bfdev_sha2_transform(ctx->state, buffer, src);
        src += BFDEV_SHA2_BLOCK_SIZE;
    }
}

export void
bfdev_sha2_update(bfdev_sha2_ctx_t *ctx, const char *data, size_t size)
{
    bfdev_sha2_base_update(ctx, data, size, sha2_transform_block);
}

export void
bfdev_sha224_finish(bfdev_sha2_ctx_t *ctx, void *hash)
{
    bfdev_sha2_base_finalize(ctx, sha2_transform_block);
    bfdev_sha2_base_finish(ctx, BFDEV_SHA224_DIGEST_WORDS, hash);
}

export void
bfdev_sha256_finish(bfdev_sha2_ctx_t *ctx, void *hash)
{
    bfdev_sha2_base_finalize(ctx, sha2_transform_block);
    bfdev_sha2_base_finish(ctx, BFDEV_SHA256_DIGEST_WORDS, hash);
}

export void
bfdev_sha224_init(bfdev_sha2_ctx_t *ctx)
{
    bfdev_sha224_digest_init(ctx->state);
    ctx->count = 0;
}

export void
bfdev_sha256_init(bfdev_sha2_ctx_t *ctx)
{
    bfdev_sha256_digest_init(ctx->state);
    ctx->count = 0;
}
