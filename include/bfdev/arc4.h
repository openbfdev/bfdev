/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARC4_H_
#define _BFDEV_ARC4_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_ARC4_KSIZE
# define BFDEV_ARC4_KSIZE 256
#endif

typedef struct bfdev_arc4_ctx bfdev_arc4_ctx_t;

struct bfdev_arc4_ctx {
    uint8_t box[BFDEV_ARC4_KSIZE];
    uint8_t proa;
    uint8_t prob;
};

extern void
bfdev_arc4_trans(bfdev_arc4_ctx_t *ctx, void *buff,
                 const void *data, size_t size);

extern void
bfdev_arc4_setkey(bfdev_arc4_ctx_t *ctx,
                  const uint8_t *key, unsigned int klen);

BFDEV_END_DECLS

#endif /* _BFDEV_ARC4_H_ */
