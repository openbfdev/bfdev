/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/sha1.h>
#include <bfdev/crypto/sha1.h>
#include <bfdev/crypto/sha1-base.h>
#include <export.h>

static void
sha1_transform_block(bfdev_sha1_ctx_t *ctx, const void *src, size_t blocks)
{
    uint32_t buffer[BFDEV_SHA1_WORKSPACE_WORDS];

    while (blocks--) {
        bfdev_sha1_transform(ctx->state, buffer, src);
        src += BFDEV_SHA1_BLOCK_SIZE;
    }
}

export void
bfdev_sha1_update(bfdev_sha1_ctx_t *ctx, const void *data, size_t size)
{
    bfdev_sha1_base_update(ctx, data, size, sha1_transform_block);
}

export void
bfdev_sha1_finish(bfdev_sha1_ctx_t *ctx, void *hash)
{
    bfdev_sha1_base_finalize(ctx, sha1_transform_block);
    bfdev_sha1_base_finish(ctx, hash);
}

export void
bfdev_sha1_init(bfdev_sha1_ctx_t *ctx)
{
    bfdev_sha1_digest_init(ctx->state);
    ctx->count = 0;
}
