/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TOKENBUCKET_H_
#define _BFDEV_TOKENBUCKET_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_tokenbucket bfdev_tokenbucket_t;

struct bfdev_tokenbucket {
    bfdev_time_t interval;
    unsigned int capacity;

    bfdev_time_t last;
    unsigned int current;
    unsigned int passed;
    unsigned int missed;
};

#define BFDEV_TOKENBUCKET_STATIC(INTERVAL, CAPACITY) { \
    .interval = (INTERVAL), .capacity = (CAPACITY), \
}

#define BFDEV_TOKENBUCKET_INIT(interval, capacity) \
    (bfdev_tokenbucket_t) BFDEV_TOKENBUCKET_STATIC(interval, capacity)

#define BFDEV_DEFINE_TOKENBUCKET(name, interval, capacity) \
    bfdev_tokenbucket_t name = BFDEV_TOKENBUCKET_INIT(interval, capacity)

static inline void
bfdev_tokenbucket_init(bfdev_tokenbucket_t *limit,
                       bfdev_time_t interval, unsigned int capacity)
{
    *limit = BFDEV_TOKENBUCKET_INIT(interval, capacity);
}

static inline void
bfdev_tokenbucket_update(bfdev_tokenbucket_t *limit, bfdev_time_t current)
{
    limit->last = current;
}

static inline void
bfdev_tokenbucket_reset(bfdev_tokenbucket_t *limit)
{
    limit->last = 0;
    limit->current = 0;
    limit->passed = 0;
    limit->missed = 0;
}

/*
 * bfdev_tokenbucket() - token bucket limiting.
 * @limit: tokenbucket state data.
 * @current: current time.
 *
 * RETURNS:
 * 0 means function will be suppressed.
 * 1 means go ahead and do it.
 */
extern bool
bfdev_tokenbucket(bfdev_tokenbucket_t *limit, bfdev_time_t current);

BFDEV_END_DECLS

#endif /* _BFDEV_TOKENBUCKET_H_ */
