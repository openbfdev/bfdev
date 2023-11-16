/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
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

static long
test_cmp(const void *node1, const void *node2, void *pdata)
{
    uintptr_t valuea, valueb;

    valuea = (uintptr_t)node1;
    valueb = (uintptr_t)node2;

    if (valuea == valueb)
        return 0;

    return valuea > valueb ? 1 : -1;
}

static long
test_find(const void *node, void *pdata)
{
    uintptr_t valuea, valueb;

    valuea = (uintptr_t)node;
    valueb = (uintptr_t)pdata;

    if (valuea == valueb)
        return 0;

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
    srand(time(NULL));
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        value = ((uint64_t)rand() << 32) | rand();
        record[count] = value;
        retval = bfdev_skiplist_insert(head, (void *)value, test_cmp, NULL);
        if (retval)
            return 1;
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Find %u Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        value = record[(unsigned long)rand() % TEST_LEN];
        node = bfdev_skiplist_find(head, test_find, (void *)value);
        if (!node)
            return 1;
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    bfdev_skiplist_destroy(head, NULL);
    free(record);

    return retval;
}
