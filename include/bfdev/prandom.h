/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PRANDOM_H_
#define _BFDEV_PRANDOM_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

struct bfdev_prandom_state {
    uint32_t s1, s2;
    uint32_t s3, s4;
};

#define BFDEV_TAUSWORTHE(seed, a, b, c, d) (    \
    (((seed) & (c)) << (d)) ^                   \
    ((((seed) << (a)) ^ (seed)) >> (b))         \
)

extern uint32_t bfdev_prandom_value(struct bfdev_prandom_state *pstate);
extern void bfdev_prandom_seed(struct bfdev_prandom_state *pstate, uint64_t seed);

static inline uint64_t
bfdev_prandom_u64(struct bfdev_prandom_state *pstate)
{
    uint32_t high = bfdev_prandom_value(pstate);
    return ((uint64_t)high << 32) + bfdev_prandom_value(pstate);
}

static inline unsigned long
bfdev_prandom_long(struct bfdev_prandom_state *pstate)
{
#if BFDEV_BITS_PER_LONG == 32
    return bfdev_prandom_value(pstate);
#else /* BITS_PER_LONG == 64 */
    return bfdev_prandom_u64(pstate);
#endif
}

BFDEV_END_DECLS

#endif /* _BFDEV_PRANDOM_H_ */
