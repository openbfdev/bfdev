/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/time.h>
#include <bfdev/timer.h>
#include <export.h>

static void
base_enqueue(struct timer_base *base, struct timer *timer, unsigned int idx, ttime_t expiry)
{
    hlist_head_add(base->wheels + idx, &timer->list);
    bit_set(base->pending_map, idx);

    if (ttime_before(expiry, base->recent_timer)) {
        base->recent_timer = expiry;
        base->timers_pending = true;
        base->next_recalc = false;
    }
}

static void
base_dequeue(struct timer *timer, bool clear)
{
    struct hlist_node *list = &timer->list;

    hlist_deluf(&timer->list);
    if (clear)
        list->pprev = NULL;

    list->next = POISON_HLIST2;
}

static bool
base_try_dequeue(struct timer_base *base, struct timer *timer, bool clear)
{
    if (!timer_check_pending(timer))
        return false;

    base_dequeue(timer, clear);
    if (hlist_check_empty(base->wheels + timer->index)) {
        bit_clr(base->pending_map, timer->index);
        base->next_recalc = true;
    }

    return true;
}


static unsigned int
collect_timers(struct timer_base *base, struct hlist_head *heads)
{
    unsigned int count, index, levels;
    struct hlist_head *vector;
    uint64_t curr;

    base->current = base->recent_timer;
    curr = base->recent_timer;

    levels = 0;
    for (count = 0; count < WHEEL_DEPTH; ++count) {
        index = (curr & LEVEL_MASK) + count * LEVEL_SIZE;

        if (bfdev_bit_test_clr(base->pending_map, index)) {
            vector = base->wheels + index;
            bfdev_hlist_move_list(vector, heads++);
            levels++;
        }

        if (curr & CLOCK_MASK)
            break;

        curr >>= CLOCK_SHIFT;
    }

    return levels;
}

static __always_inline int
next_pending_bucket(struct timer_base *base, unsigned int index, ttime_t clk)
{
    unsigned int pos, start = index + clk;
    unsigned int end = index + LEVEL_SIZE;

    pos = find_next_bit(base->pending_map, end, start);
    if (pos < end)
        return pos - start;

    pos = find_next_bit(base->pending_map, start, index);
    return pos < start ? pos + LEVEL_SIZE - start : -1;
}

static ttime_t
next_recent_timer(struct timer_base *base)
{
    unsigned int depth, index = 0;
    uint64_t curr, next;

    curr = base->current;
    next = curr + TIMER_EXPIRE_DELTA_MAX;

    for (depth = 0; depth < WHEEL_DEPTH; ++depth, index += LEVEL_SIZE) {
        int pos = next_pending_bucket(base, index, curr & LEVEL_MASK);
        int level_clk = curr & CLOCK_MASK;

        if (pos >= 0) {
            uint64_t tmp = curr + pos;

            tmp <<= LEVEL_SHIFT(depth);
            if (ttime_before(tmp, next))
                next = tmp;

            if (pos <= ((CLOCK_DIV - level_clk) & CLOCK_MASK))
                break;
        }

        curr >>= CLOCK_SHIFT;
        curr += !!level_clk;
    }

    base->next_recalc = false;
    base->timers_pending = !(next == base->current + TIMER_EXPIRE_DELTA_MAX);

    return next;
}

static void
timer_expire(struct timer_base *base, struct hlist_head *head, void *pdata)
{
    struct timer *timer;

    while (!hlist_check_empty(head)) {
        timer = hlist_first_entry(head, struct timer, list);
        base_dequeue(timer, true);

        base->running = timer;
        timer->expire(timer->data, pdata);
    }
}

int base_do_timers(bfdev_timewheel_t *base, bfdev_time_t time, void *pdata)
{
    bfdev_hlist_head_t heads[BFDEV_TIMER_WHEEL_DEPTH];
    int retval;

    if (bfdev_time_before(time, base->recent_timer))
        return -BFDEV_ENOERR;

    while (bfdev_time_after_equal(time, base->current) &&
           bfdev_time_after_equal(time, base->recent_timer)) {
        unsigned int levels;

        levels = collect_timers(base, heads);
        BFDEV_WARN_ON(!levels && !base->next_recalc && base->timers_pending);

        base->current++;
        base->recent_timer = next_recent_timer(base);

        while (levels--) {
            retval = timer_expire(base, heads + levels);
            if (bfdev_unlikely(retval))
                return retval;
        }
    }

    return -BFDEV_ENOERR;
}
