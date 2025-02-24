/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SHA1_H_
#define _BFDEV_SHA1_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

#define BFDEV_SHA1_DIGEST0 0x67452301UL
#define BFDEV_SHA1_DIGEST1 0xefcdab89UL
#define BFDEV_SHA1_DIGEST2 0x98badcfeUL
#define BFDEV_SHA1_DIGEST3 0x10325476UL
#define BFDEV_SHA1_DIGEST4 0xc3d2e1f0UL

#define BFDEV_SHA1_BLOCK_SIZE 64
#define BFDEV_SHA1_WORKSPACE_WORDS 16

#define BFDEV_SHA1_DIGEST_SIZE 20
#define BFDEV_SHA1_DIGEST_WORDS (BFDEV_SHA1_DIGEST_SIZE / BFDEV_BYTES_PER_U32)

typedef struct bfdev_sha1_ctx bfdev_sha1_ctx_t;

struct bfdev_sha1_ctx {
    uint32_t state[BFDEV_SHA1_DIGEST_WORDS];
    uint8_t buffer[BFDEV_SHA1_BLOCK_SIZE];
    unsigned long count;
};

extern void
bfdev_sha1_update(bfdev_sha1_ctx_t *ctx, const void *data, size_t size);

extern void
bfdev_sha1_finish(bfdev_sha1_ctx_t *ctx, void *hash);

extern void
bfdev_sha1_init(bfdev_sha1_ctx_t *ctx);

BFDEV_END_DECLS

#endif /* _BFDEV_SHA1_H_ */
