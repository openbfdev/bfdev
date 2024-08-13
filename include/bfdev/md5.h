/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MD5_H_
#define _BFDEV_MD5_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

#define BFDEV_MD5_DIGEST_SIZE       16
#define BFDEV_MD5_HMAC_BLOCK_SIZE   64
#define BFDEV_MD5_BLOCK_WORDS       16
#define BFDEV_MD5_HASH_WORDS        4

#define BFDEV_MD5_H0 0x67452301UL
#define BFDEV_MD5_H1 0xefcdab89UL
#define BFDEV_MD5_H2 0x98badcfeUL
#define BFDEV_MD5_H3 0x10325476UL

typedef struct bfdev_md5_ctx bfdev_md5_ctx_t;

struct bfdev_md5_ctx {
	uint32_t digest[BFDEV_MD5_HASH_WORDS];
	uint32_t block[BFDEV_MD5_BLOCK_WORDS];
	unsigned long count;
};

extern void
bfdev_md5_update(bfdev_md5_ctx_t *ctx, const char *data, size_t size);

extern void
bfdev_md5_finish(bfdev_md5_ctx_t *ctx, void *hash);

extern void
bfdev_md5_init(bfdev_md5_ctx_t *ctx);

BFDEV_END_DECLS

#endif /* _BFDEV_MD5_H_ */
