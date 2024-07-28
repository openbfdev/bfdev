/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_PRANDOM_H_
#define _BFDEV_PRANDOM_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_PRANDOM_SEED
# define BFDEV_PRANDOM_SEED 0x408c3c09UL
#endif

#ifndef BFDEV_PRANDOM_WARMUP
# define BFDEV_PRANDOM_WARMUP 10
#endif

typedef struct bfdev_prandom bfdev_prandom_t;

struct bfdev_prandom {
    uint32_t s1, s2;
    uint32_t s3, s4;
};

#define BFDEV_PRANDOM_STATIC() { \
    .s1 = BFDEV_PRANDOM_SEED, .s2 = BFDEV_PRANDOM_SEED, \
    .s3 = BFDEV_PRANDOM_SEED, .s4 = BFDEV_PRANDOM_SEED, \
}

#define BFDEV_PRANDOM_INIT() \
    (bfdev_prandom_t) BFDEV_PRANDOM_STATIC()

#define BFDEV_DEFINE_PRANDOM(name) \
    bfdev_prandom_t name = BFDEV_PRANDOM_INIT()

/* Tausworthe generators */
#define BFDEV_TAUSWORTHE(seed, a, b, c, d) (    \
    (((seed) & (c)) << (d)) ^                   \
    ((((seed) << (a)) ^ (seed)) >> (b))         \
)

static inline void
bfdev_prandom_init(bfdev_prandom_t *pstate)
{
    *pstate = BFDEV_PRANDOM_INIT();
}

/**
 * bfdev_prandom_seed() - set seed for prandom_state.
 * @state: pointer to state structure to receive the seed.
 * @seed: arbitrary 64-bit value to use as a seed.
 */
extern void
bfdev_prandom_seed(bfdev_prandom_t *pstate, uint64_t seed);

/**
 * bfdev_prandom_value() - seeded pseudo-random number generator.
 * @state: pointer to state structure holding seeded state.
 */
extern uint32_t
bfdev_prandom_value(bfdev_prandom_t *pstate);

static inline uint64_t
bfdev_prandom_u64(bfdev_prandom_t *pstate)
{
    uint32_t high, low;

    high = bfdev_prandom_value(pstate);
    low = bfdev_prandom_value(pstate);

    return ((uint64_t)high << 32) | low;
}

static inline unsigned long
bfdev_prandom_long(bfdev_prandom_t *pstate)
{
#if BFDEV_BITS_PER_LONG == 32
    return bfdev_prandom_value(pstate);
#else /* BITS_PER_LONG == 64 */
    return bfdev_prandom_u64(pstate);
#endif
}

BFDEV_END_DECLS

#endif /* _BFDEV_PRANDOM_H_ */
