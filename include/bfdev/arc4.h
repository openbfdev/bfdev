/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
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
    bfdev_u8 box[BFDEV_ARC4_KSIZE];
    bfdev_u8 proa;
    bfdev_u8 prob;
};

extern void
bfdev_arc4_trans(bfdev_arc4_ctx_t *ctx, void *buff,
                 const void *data, bfdev_size_t size);

extern void
bfdev_arc4_setkey(bfdev_arc4_ctx_t *ctx,
                  const bfdev_u8 *key, unsigned int klen);

BFDEV_END_DECLS

#endif /* _BFDEV_ARC4_H_ */
