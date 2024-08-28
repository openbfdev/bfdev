/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RATELIMIT_H_
#define _BFDEV_RATELIMIT_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_ratelimit bfdev_ratelimit_t;

/**
 * struct bfdev_ratelimit - describe ratelimit status.
 * @begin: start time of current management granularity.
 * @interval: manage time granularity of the maximum number of transfers.
 * @burst: maximum number of transfers allowed in @interval period.
 * @passed: number of data passed in compliance with rate limit.
 * @missed: number of lost data exceeding the rate limit.
 */
struct bfdev_ratelimit {
    bfdev_time_t begin;
    bfdev_time_t interval;
    unsigned int burst;
    unsigned int passed;
    unsigned int missed;
};

#define BFDEV_RATELIMIT_STATIC(INTERVAL, BURST) { \
    .interval = (INTERVAL), .burst = (BURST) \
}

#define BFDEV_RATELIMIT_INIT(interval, burst) \
    (bfdev_ratelimit_t) BFDEV_RATELIMIT_STATIC(interval, burst)

#define BFDEV_DEFINE_RATELIMIT(name, interval, burst) \
    bfdev_ratelimit_t name = BFDEV_RATELIMIT_INIT(interval, burst)

static inline void
bfdev_ratelimit_init(bfdev_ratelimit_t *limit,
                     bfdev_time_t interval, unsigned int burst)
{
    *limit = BFDEV_RATELIMIT_INIT(interval, burst);
}

static inline void
bfdev_ratelimit_reset(bfdev_ratelimit_t *limit)
{
    limit->missed = 0;
}

/*
 * bfdev_ratelimit() - rate limiting.
 * @limit: ratelimit state data.
 * @current: current time.
 *
 * RETURNS:
 * 0 means function will be suppressed.
 * 1 means go ahead and do it.
 */
extern bool
bfdev_ratelimit(bfdev_ratelimit_t *limit, bfdev_time_t current);

BFDEV_END_DECLS

#endif /* _BFDEV_RATELIMIT_H_ */
