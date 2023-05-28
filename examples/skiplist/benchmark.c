/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/times.h>
#include <bfdev/skiplist.h>

#define TEST_DEPTH 32
#define TEST_LEN 1000000

static inline void time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static inline long skiplist_bench_cmp(const void *nodea, const void *nodeb)
{
    uintptr_t valuea = (uintptr_t)nodea;
    uintptr_t valueb = (uintptr_t)nodeb;
    if (valuea == valueb) return 0;
    return valuea > valueb ? 1 : -1;
}

int main(void)
{
    struct bfdev_skip_head *head;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    uintptr_t value;
    int retval;

    ticks = sysconf(_SC_CLK_TCK);
    head = bfdev_skiplist_create(TEST_DEPTH);
    if (!head)
        return 1;

    printf("Insert %u Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        value = ((unsigned long)rand() << 32) | rand();
        retval = bfdev_skiplist_insert(head, (void *)value, skiplist_bench_cmp);
        if (retval)
            goto error;
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

error:
    bfdev_skiplist_destroy(head, NULL);
    return retval;
}
