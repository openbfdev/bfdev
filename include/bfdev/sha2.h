/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SHA2_H_
#define _BFDEV_SHA2_H_

#include <bfdev/config.h>
#include <bfdev/types.h>

BFDEV_BEGIN_DECLS

#define BFDEV_SHA224_DIGEST0 0xc1059ed8UL
#define BFDEV_SHA224_DIGEST1 0x367cd507UL
#define BFDEV_SHA224_DIGEST2 0x3070dd17UL
#define BFDEV_SHA224_DIGEST3 0xf70e5939UL
#define BFDEV_SHA224_DIGEST4 0xffc00b31UL
#define BFDEV_SHA224_DIGEST5 0x68581511UL
#define BFDEV_SHA224_DIGEST6 0x64f98fa7UL
#define BFDEV_SHA224_DIGEST7 0xbefa4fa4UL

#define BFDEV_SHA256_DIGEST0 0x6a09e667UL
#define BFDEV_SHA256_DIGEST1 0xbb67ae85UL
#define BFDEV_SHA256_DIGEST2 0x3c6ef372UL
#define BFDEV_SHA256_DIGEST3 0xa54ff53aUL
#define BFDEV_SHA256_DIGEST4 0x510e527fUL
#define BFDEV_SHA256_DIGEST5 0x9b05688cUL
#define BFDEV_SHA256_DIGEST6 0x1f83d9abUL
#define BFDEV_SHA256_DIGEST7 0x5be0cd19UL

#define BFDEV_SHA2_BLOCK_SIZE 64
#define BFDEV_SHA2_WORKSPACE_WORDS 64

#define BFDEV_SHA224_DIGEST_SIZE 28
#define BFDEV_SHA224_DIGEST_WORDS (BFDEV_SHA224_DIGEST_SIZE / BFDEV_BYTES_PER_U32)

#define BFDEV_SHA256_DIGEST_SIZE 32
#define BFDEV_SHA256_DIGEST_WORDS (BFDEV_SHA256_DIGEST_SIZE / BFDEV_BYTES_PER_U32)

typedef struct bfdev_sha2_ctx bfdev_sha2_ctx_t;

struct bfdev_sha2_ctx {
    uint32_t state[BFDEV_SHA256_DIGEST_WORDS];
    uint8_t buffer[BFDEV_SHA2_BLOCK_SIZE];
    unsigned long count;
};

extern void
bfdev_sha2_update(bfdev_sha2_ctx_t *ctx, const char *data, size_t size);

extern void
bfdev_sha224_finish(bfdev_sha2_ctx_t *ctx, void *hash);

extern void
bfdev_sha256_finish(bfdev_sha2_ctx_t *ctx, void *hash);

extern void
bfdev_sha224_init(bfdev_sha2_ctx_t *ctx);

extern void
bfdev_sha256_init(bfdev_sha2_ctx_t *ctx);

BFDEV_END_DECLS

#endif /* _BFDEV_SHA2_H_ */
