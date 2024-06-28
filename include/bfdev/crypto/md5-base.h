/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(C) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_MD5_BASE_H_
#define _BFDEV_CRYPTO_MD5_BASE_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/string.h>
#include <bfdev/bits.h>
#include <bfdev/byteorder.h>
#include <bfdev/md5.h>

BFDEV_BEGIN_DECLS

typedef void (*bfdev_md5_bfn_t)
(bfdev_md5_ctx_t *ctx);

static inline void
bfdev_md5_base_update(bfdev_md5_ctx_t *ctx, const void *data,
                      size_t size, bfdev_md5_bfn_t func)
{
	unsigned long avail;

	avail = sizeof(ctx->block) - (ctx->count & 0x3f);
	ctx->count += size;

	if (avail > size) {
		bfport_memcpy((void *)ctx->block + (sizeof(ctx->block) - avail),
					  data, size);
		return;
	}

	bfport_memcpy((char *)ctx->block + (sizeof(ctx->block) - avail),
				  data, avail);
	func(ctx);
	data += avail;
	size -= avail;

	while (size >= sizeof(ctx->block)) {
		memcpy(ctx->block, data, sizeof(ctx->block));
		func(ctx);
		data += sizeof(ctx->block);
		size -= sizeof(ctx->block);
	}

	memcpy(ctx->block, data, size);
}

static inline void
bfdev_md5_base_finalize(bfdev_md5_ctx_t *ctx, bfdev_md5_bfn_t func)
{
	unsigned long offset;
	int padding;
	char *ptr;

	offset = ctx->count & 0x3f;
	ptr = (char *)ctx->block + offset;
	*ptr++ = 0x80;

	padding = 56 - (offset + 1);
	if (padding < 0) {
		bfport_memset(ptr, 0, padding + BFDEV_BYTES_PER_U64);
		func(ctx);
		ptr = (char *)ctx->block;
		padding = 56;
	}

	bfport_memset(ptr, 0, padding);
	ctx->block[14] = ctx->count << 3;
	ctx->block[15] = ctx->count >> 29;
	bfdev_le32_to_cpu_array(ctx->block, (bfdev_le32 *)ctx->block, (sizeof(ctx->block) -
							BFDEV_BYTES_PER_U64) / BFDEV_BYTES_PER_U32);
	func(ctx);
}

static inline void
bfdev_md5_base_final(bfdev_md5_ctx_t *ctx, void *buff)
{
	bfdev_cpu_to_le32_array((bfdev_le32 *)ctx->digest, ctx->digest,
						    sizeof(ctx->digest) / BFDEV_BYTES_PER_U32);
	bfport_memcpy(buff, ctx->digest, sizeof(ctx->digest));
}

static inline void
bfdev_md5_digest_init(uint32_t *digest)
{
	digest[0] = BFDEV_MD5_H0;
	digest[1] = BFDEV_MD5_H1;
	digest[2] = BFDEV_MD5_H2;
	digest[3] = BFDEV_MD5_H3;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CRYPTO_MD5_BASE_H_ */
