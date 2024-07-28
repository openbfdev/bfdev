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
prandom_setup(bfdev_prandom_t *pstate, uint64_t seed)
{
    uint32_t s1, s2, s3, s4;

    seed = bfdev_lower_32_bits((seed >> 32) ^ (seed << 10) ^ seed);
    s1 = seed_minimum(seed,   2U);
    s2 = seed_minimum(seed,   8U);
    s3 = seed_minimum(seed,  16U);
    s4 = seed_minimum(seed, 128U);

    pstate->s1 = s1;
    pstate->s2 = s2;
    pstate->s3 = s3;
    pstate->s4 = s4;
}

static __bfdev_always_inline void
prandom_value(bfdev_prandom_t *pstate, uint32_t buff[4])
{
    buff[0] = BFDEV_TAUSWORTHE(pstate->s1,  6U, 13U, 4294967294U, 18U);
    buff[1] = BFDEV_TAUSWORTHE(pstate->s2,  2U, 27U, 4294967288U,  2U);
    buff[2] = BFDEV_TAUSWORTHE(pstate->s3, 13U, 21U, 4294967280U,  7U);
    buff[3] = BFDEV_TAUSWORTHE(pstate->s4,  3U, 12U, 4294967168U, 13U);

    pstate->s1 = buff[0];
    pstate->s2 = buff[1];
    pstate->s3 = buff[2];
    pstate->s4 = buff[3];
}

static __bfdev_always_inline void
prandom_warmup(bfdev_prandom_t *pstate)
{
    unsigned int count;
    uint32_t buff[4];

    for (count = 0; count < BFDEV_PRANDOM_WARMUP; ++count)
        prandom_value(pstate, buff);
}

export uint32_t
bfdev_prandom_value(bfdev_prandom_t *pstate)
{
    uint32_t value, buff[4];

    prandom_value(pstate, buff);
    value = buff[0] ^ buff[1] ^ buff[2] ^ buff[3];

    return value;
}

export void
bfdev_prandom_seed(bfdev_prandom_t *pstate, uint64_t seed)
{
    prandom_setup(pstate, seed);
    prandom_warmup(pstate);
}
