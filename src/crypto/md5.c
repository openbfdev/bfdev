/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/md5.h>
#include <bfdev/crypto/md5.h>
#include <bfdev/crypto/md5-base.h>
#include <export.h>

static inline void
md5_transform_block(bfdev_md5_ctx_t *ctx)
{
	bfdev_le32_to_cpu_array(ctx->block, (bfdev_le32 *)ctx->block,
                            sizeof(ctx->block) / BFDEV_BYTES_PER_U32);
	bfdev_md5_transform(ctx->digest, ctx->block);
}

export void
bfdev_md5_update(bfdev_md5_ctx_t *ctx, const char *data, size_t size)
{
    bfdev_md5_base_update(ctx, data, size, md5_transform_block);
}

export void
bfdev_md5_finish(bfdev_md5_ctx_t *ctx, void *hash)
{
    bfdev_md5_base_finalize(ctx, md5_transform_block);
    bfdev_md5_base_final(ctx, hash);
}

export void
bfdev_md5_init(bfdev_md5_ctx_t *ctx)
{
    bfdev_md5_digest_init(ctx->digest);
    ctx->count = 0;
}
