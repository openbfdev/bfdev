/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <bfdev/list.h>

#define LIST_DEBUG 0
#define TEST_LEN 1000000
static BFDEV_LIST_HEAD(demo_list);

struct benchmark {
    struct bfdev_list_head list;
    unsigned int num;
    unsigned long data;
};

#define list_to_demo(node) \
    bfdev_list_entry(node, struct benchmark, list)

#if LIST_DEBUG
static void node_dump(struct benchmark *node)
{
    printf("\t%08d: data 0x%016lx\n", node->num, node->data);
}
#else
# define node_dump(node) ((void)(node))
#endif

static void time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("\treal time: %lf\n", (stop - start) / (double)ticks);
    printf("\tuser time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("\tkern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static long
demo_cmp(const struct bfdev_list_head *a,
         const struct bfdev_list_head *b, void *pdata)
{
    struct benchmark *demo_a = list_to_demo(a);
    struct benchmark *demo_b = list_to_demo(b);
    return demo_a->data - demo_b->data;
}

int main(void)
{
    struct benchmark *node, *tmp;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    int ret = 0;

    ticks = sysconf(_SC_CLK_TCK);

    printf("Generate %d Node:\n", TEST_LEN);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if ((ret = !node)) {
            printf("insufficient memory\n");
            goto error;
        }

        node->num = count;
        node->data = ((unsigned long)rand() << 32) | rand();
        node_dump(node);

        bfdev_list_add(&demo_list, &node->list);
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    printf("List sort:\n");
    bfdev_list_qsort(&demo_list, demo_cmp, NULL);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    printf("List for each:\n");
    bfdev_list_for_each_entry(node, &demo_list, list)
        node_dump(node);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Deletion All Node...\n");
error:
    bfdev_list_for_each_entry_safe(node, tmp, &demo_list, list)
        free(node);

    if (!ret)
        printf("Done.\n");

    return ret;
}
