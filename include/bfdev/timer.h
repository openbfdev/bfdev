/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TIMER_H_
#define _BFDEV_TIMER_H_

#include <bfdev/config.h>
#include <bfdev/hlist.h>
#include <bfdev/bitmap.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_TIMER_CLOCK_SHIFT
# define BFDEV_TIMER_CLOCK_SHIFT 3
#endif

#ifndef BFDEV_TIMER_LEVEL_WIDTH
# define BFDEV_TIMER_LEVEL_WIDTH 6
#endif

#ifndef BFDEV_TIMER_WHEEL_DEPTH
# define BFDEV_TIMER_WHEEL_DEPTH 8
#endif

#define BFDEV_TIMER_CLOCK_DIV   BFDEV_BIT(BFDEV_TIMER_CLOCK_SHIFT)
#define BFDEV_TIMER_CLOCK_MASK  (BFDEV_TIMER_CLOCK_DIV - 1)

#define BFDEV_TIMER_LEVEL_SIZE  BFDEV_BIT(BFDEV_TIMER_LEVEL_WIDTH)
#define BFDEV_TIMER_LEVEL_MASK  (BFDEV_TIMER_LEVEL_SIZE - 1)
#define BFDEV_TIMER_WHEEL_LEN   (BFDEV_TIMER_LEVEL_SIZE * BFDEV_TIMER_WHEEL_DEPTH)

#define BFDEV_TIMER_LEVEL_SHIFT(lvl) \
    ((lvl) * BFDEV_TIMER_CLOCK_SHIFT)

#define BFDEV_TIMER_LEVEL_GAIN(lvl) \
    (1UL << BFDEV_TIMER_LEVEL_SHIFT(lvl))

#define BFDEV_TIMER_LEVEL_OFFS(lvl) \
    (BFDEV_TIMER_LEVEL_SIZE * (lvl))

#define BFDEV_TIMER_LEVEL_START(lvl) \
    (BFDEV_TIMER_LEVEL_SIZE << BFDEV_TIMER_LEVEL_SHIFT((lvl) - 1))

#define BFDEV_TIMER_TIMEOUT_CUT \
    (BFDEV_TIMER_LEVEL_START(BFDEV_TIMER_WHEEL_DEPTH))

#define BFDEV_TIMER_TIMEOUT_MAX \
    (BFDEV_TIMER_TIMEOUT_CUT - BFDEV_TIMER_LEVEL_GAIN(BFDEV_TIMER_WHEEL_DEPTH - 1))

typedef struct bfdev_timer bfdev_timer_t;
typedef struct bfdev_timewheel bfdev_timewheel_t;

typedef void
(*bfdev_timer_entry_t)(void *data, void *pdata);

struct bfdev_timer {
    bfdev_hlist_node_t list;
    bfdev_time_t delta;
    bfdev_time_t expires;

    unsigned int index;
    unsigned long flags;

    bfdev_timer_entry_t entry;
    void *data;
};

struct bfdev_timewheel {
    bool next_recalc;
    bool timers_pending;

    bfdev_time_t current;
    bfdev_time_t recent_timer;

    BFDEV_DEFINE_BITMAP(pending_map, BFDEV_TIMER_WHEEL_LEN);
    bfdev_hlist_head_t wheels[BFDEV_TIMER_WHEEL_LEN];
};

extern int
bfdev_timer_pending(bfdev_timewheel_t *wheel, bfdev_timer_t *timer);

extern int
bfdev_timer_cancle(bfdev_timewheel_t *wheel, bfdev_timer_t *timer);

extern void
bfdev_timer_handle(bfdev_timewheel_t *wheel, bfdev_time_t time, void *pdata);

extern void
bfdev_timer_init(bfdev_timewheel_t *wheel);

BFDEV_END_DECLS

#endif /* _BFDEV_TIMER_H_ */
