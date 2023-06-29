/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/prandom.h>
#include <bfdev/uplower.h>
#include <export.h>

static __bfdev_always_inline uint32_t
prandom_seed_minimum(uint32_t x, uint32_t m)
{
    return (x < m) ? x + m : x;
}

export uint32_t
bfdev_prandom_value(struct bfdev_prandom_state *pstate)
{
    pstate->s1 = BFDEV_TAUSWORTHE(pstate->s1,  6U, 13U, 4294967294U, 18U);
    pstate->s2 = BFDEV_TAUSWORTHE(pstate->s2,  2U, 27U, 4294967288U,  2U);
    pstate->s3 = BFDEV_TAUSWORTHE(pstate->s3, 13U, 21U, 4294967280U,  7U);
    pstate->s4 = BFDEV_TAUSWORTHE(pstate->s4,  3U, 12U, 4294967168U, 13U);
    return pstate->s1 ^ pstate->s2 ^ pstate->s3 ^ pstate->s4;
}

static void
bfdev_prandom_setup(struct bfdev_prandom_state *pstate, uint64_t seed)
{
    seed = bfdev_lower_32_bits((seed >> 32) ^ (seed << 10) ^ seed);
    pstate->s1 = prandom_seed_minimum(seed,   2U);
    pstate->s2 = prandom_seed_minimum(seed,   8U);
    pstate->s3 = prandom_seed_minimum(seed,  16U);
    pstate->s4 = prandom_seed_minimum(seed, 128U);
}

static __bfdev_always_inline void
bfdev_prandom_warmup(struct bfdev_prandom_state *pstate)
{
    unsigned int count;
    for (count = 0; count < 10; ++count)
        bfdev_prandom_value(pstate);
}

export void
bfdev_prandom_seed(struct bfdev_prandom_state *pstate, uint64_t seed)
{
    bfdev_prandom_setup(pstate, seed);
    bfdev_prandom_warmup(pstate);
}
