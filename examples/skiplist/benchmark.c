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

static inline void
time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static inline long
skiplist_bench_cmp(const void *nodea, const void *nodeb)
{
    uintptr_t valuea = (uintptr_t)nodea;
    uintptr_t valueb = (uintptr_t)nodeb;
    if (valuea == valueb) return 0;
    return valuea > valueb ? 1 : -1;
}

static inline long
skiplist_bench_find(const void *nodea, const void *nodeb)
{
    uintptr_t valuea = (uintptr_t)nodea;
    uintptr_t valueb = (uintptr_t)nodeb;
    if (valuea == valueb) return 0;
    return valuea > valueb ? 1 : -1;
}

int main(void)
{
    struct bfdev_skip_head *head;
    struct bfdev_skip_node *node;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    uintptr_t value, *record;
    int retval;

    record = malloc(TEST_LEN * sizeof(*record));
    if (!record)
        return 1;

    ticks = sysconf(_SC_CLK_TCK);
    head = bfdev_skiplist_create(NULL, TEST_DEPTH);
    if (!head)
        return 1;

    printf("Insert %u Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        value = ((unsigned long)rand() << 32) | rand();
        record[count] = value;
        retval = bfdev_skiplist_insert(head, (void *)value, skiplist_bench_cmp);
        if (retval)
            goto error;
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Find %u Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        value = record[(unsigned long)rand() % TEST_LEN];
        node = bfdev_skiplist_find(head, (void *)value, skiplist_bench_find);
        if (!node)
            goto error;
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

error:
    bfdev_skiplist_destroy(head, NULL);
    free(record);
    return retval;
}
