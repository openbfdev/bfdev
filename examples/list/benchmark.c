/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/list.h>
#include <unistd.h>
#include <sys/times.h>

#define LIST_DEBUG 0
#define TEST_LEN 1000000
static LIST_HEAD(demo_list);

struct list_demo {
    struct list_head list;
    unsigned int num;
    unsigned long data;
};

#define list_to_demo(node) \
    list_entry(node, struct list_demo, list)

#if LIST_DEBUG
static void node_dump(struct list_demo *node)
{
    printf("  %08d: data 0x%016lx\n", node->num, node->data);
}
#else
# define node_dump(node) ((void)(node))
#endif

static void time_dump(int ticks, clock_t start, clock_t stop, struct tms *start_tms, struct tms *stop_tms)
{
    printf("  real time: %lf\n", (stop - start) / (double)ticks);
    printf("  user time: %lf\n", (stop_tms->tms_utime - start_tms->tms_utime) / (double)ticks);
    printf("  kern time: %lf\n", (stop_tms->tms_stime - start_tms->tms_stime) / (double)ticks);
}

static long demo_cmp(struct list_head *a, struct list_head *b, void *pdata)
{
    struct list_demo *demo_a = list_to_demo(a);
    struct list_demo *demo_b = list_to_demo(b);
    return demo_a->data - demo_b->data;
}

int main(void)
{
    struct list_demo *node, *tmp;
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

        list_add(&demo_list, &node->list);
    }
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    printf("List sort:\n");
    list_qsort(&demo_list, demo_cmp, NULL);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    printf("List for each:\n");
    list_for_each_entry(node, &demo_list, list)
        node_dump(node);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Deletion All Node...\n");
error:
    list_for_each_entry_safe(node, tmp, &demo_list, list)
        free(node);

    if (!ret)
        printf("Done.\n");

    return ret;
}
