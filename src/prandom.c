/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/prandom.h>
#include <bfdev/uplower.h>
#include <export.h>

static __bfdev_always_inline uint32_t
seed_minimum(uint32_t x, uint32_t m)
{
    return (x < m) ? x + m : x;
}

static void
prandom_setup(bfdev_prandom_state_t *pstate, uint64_t seed)
{
    seed = bfdev_lower_32_bits((seed >> 32) ^ (seed << 10) ^ seed);
    pstate->s1 = seed_minimum(seed,   2U);
    pstate->s2 = seed_minimum(seed,   8U);
    pstate->s3 = seed_minimum(seed,  16U);
    pstate->s4 = seed_minimum(seed, 128U);
}

static __bfdev_always_inline void
prandom_warmup(bfdev_prandom_state_t *pstate)
{
    unsigned int count;
    for (count = 0; count < 10; ++count)
        bfdev_prandom_value(pstate);
}

export uint32_t
bfdev_prandom_value(bfdev_prandom_state_t *pstate)
{
    uint32_t s1, s2, s3, s4;

    s1 = BFDEV_TAUSWORTHE(pstate->s1,  6U, 13U, 4294967294U, 18U);
    s2 = BFDEV_TAUSWORTHE(pstate->s2,  2U, 27U, 4294967288U,  2U);
    s3 = BFDEV_TAUSWORTHE(pstate->s3, 13U, 21U, 4294967280U,  7U);
    s4 = BFDEV_TAUSWORTHE(pstate->s4,  3U, 12U, 4294967168U, 13U);

    pstate->s1 = s1;
    pstate->s2 = s2;
    pstate->s3 = s3;
    pstate->s4 = s4;

    return s1 ^ s2 ^ s3 ^ s4;
}

export void
bfdev_prandom_seed(bfdev_prandom_state_t *pstate, uint64_t seed)
{
    prandom_setup(pstate, seed);
    prandom_warmup(pstate);
}
