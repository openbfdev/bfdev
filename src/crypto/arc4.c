/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/arc4.h>
#include <export.h>

static __bfdev_always_inline void
arc4_transform(bfdev_arc4_ctx_t *ctx, bfdev_u8 *buff,
               const bfdev_u8 *data, bfdev_size_t size)
{
    bfdev_u8 proa, prob;
    bfdev_u8 tx, ty, tb;
    bfdev_u8 vx, vy;

    proa = ctx->proa;
    prob = ctx->prob;

    vx = ctx->box[proa];
    prob += vx;
    vy = ctx->box[prob];

    for (;;) {
        ctx->box[prob] = vx;
        vx += vy;
        ctx->box[proa] = vy;
        proa++;

        tx = ctx->box[proa];
        tb = prob + tx;
        ty = ctx->box[tb];
        *buff++ = *data++ ^ ctx->box[vx];

        if (!--size)
            break;

        prob = tb;
        vx = tx;
        vy = ty;
    }

    ctx->proa = proa;
    ctx->prob = prob;
}

export void
bfdev_arc4_trans(bfdev_arc4_ctx_t *ctx, void *buff,
                 const void *data, bfdev_size_t size)
{
    if (bfdev_unlikely(!size))
        return;

    arc4_transform(ctx, buff, data, size);
}

export void
bfdev_arc4_setkey(bfdev_arc4_ctx_t *ctx,
                  const bfdev_u8 *key, unsigned int klen)
{
    bfdev_u8 value, loop;
    unsigned int count;

    if (bfdev_unlikely(!klen))
        return;

    ctx->proa = 1;
    ctx->prob = 0;

    for (count = 0; count < BFDEV_ARC4_KSIZE; ++count)
        ctx->box[count] = count;
    loop = 0;

    for (count = 0; count < BFDEV_ARC4_KSIZE; ++count) {
        value = ctx->box[count];
        loop += key[count % klen] + value;
        ctx->box[count] = ctx->box[loop];
        ctx->box[loop] = value;
    }
}
