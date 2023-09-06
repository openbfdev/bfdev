/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <bfdev/radix.h>

#define TEST_LOOP 1000000
BFDEV_RADIX_ROOT(bench_root, unsigned int, NULL);

static void
time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

int main(void)
{
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    unsigned int *data;
    int retval;

    printf("Generate %u Node:\n", TEST_LOOP);
    data = malloc(sizeof(*data) * TEST_LOOP);
    if (!data) {
        printf("Insufficient Memory!\n");
        return 1;
    }

    for (count = 0; count < TEST_LOOP; ++count)
        data[count] = rand();

    retval = bfdev_radix_charge(&bench_root, 0, TEST_LOOP);
    if (retval) {
        printf("Insufficient Memory!\n");
        return retval;
    }

    printf("Alloc Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LOOP; ++count) {
        unsigned int *block;
        block = bfdev_radix_alloc(&bench_root, count);
        *block = data[count];
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Find Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LOOP; ++count) {
        unsigned int *block;
        block = bfdev_radix_find(&bench_root, count);
        if (!block || *block != data[count]) {
            printf("Data verification failed!\n");
            return 1;
        }
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Free Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_radix_free(&bench_root, count);
        if (retval) {
            printf("Sequence error!\n");
            return 1;
        }
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    count = bench_root.tree.level;
    printf("\tradix level: %u\n", count);
    if (count) {
        printf("Pruning error!\n");
        return 1;
    }

    printf("Done.\n");
    bfdev_radix_destory(&bench_root);
    free(data);

    return 0;
}
