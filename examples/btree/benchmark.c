/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/btree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

#define BTREE_DEBUG 0
#define TEST_LEN 1000000

struct bench_node {
    unsigned int num;
    unsigned long data;
};

#if BTREE_DEBUG
static void node_dump(struct bench_node *node)
{
    printf("\t%04d: ", node->num);
    printf("data %#018lx ", node->data);
    printf("\n");
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

int main(void)
{
    struct bench_node *node;
    struct tms start_tms, stop_tms;
    clock_t start, stop;
    unsigned int count, ticks;
    uintptr_t key;
    void *block;

    BFDEV_BTREE_ROOT(bench_root, &bfdev_btree_layoutptr,
        bfdev_btree_alloc, bfdev_btree_free, bfdev_btree_key_find,
        NULL, NULL, NULL
    );

    node = block = malloc(sizeof(*node) * TEST_LEN);
    if (!block) {
        printf("Insufficient Memory!\n");
        return 1;
    }

    printf("Generate %u Node:\n", TEST_LEN);
    for (count = 0; count < TEST_LEN; ++count) {
        node[count].num = count + 1;
        node[count].data = ((unsigned long)rand() << 32) | rand();
#if BTREE_DEBUG
        printf("\t%08d: 0x%016lx\n", node->num, node->data);
#endif
    }

    printf("Insert Nodes:\n");
    ticks = sysconf(_SC_CLK_TCK);
    start = times(&start_tms);
    for (count = 0; count < TEST_LEN; ++count)
        bfdev_btree_insert(&bench_root, &node[count].data, node);
    stop = times(&stop_tms);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    count = 0;
    printf("Btree Iteration:\n");
    bfdev_btree_for_each(&bench_root, &key, node) {
        node_dump(node);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    start = times(&start_tms);
    count = 0;
    printf("Btree Reverse Iteration:\n");
    bfdev_btree_for_each_reverse(&bench_root, &key, node) {
        node_dump(node);
        count++;
    }
    stop = times(&stop_tms);
    printf("\ttotal num: %u\n", count);
    time_dump(ticks, start, stop, &start_tms, &stop_tms);

    printf("Done.\n");
    bfdev_btree_destroy(&bench_root);
    free(block);

    return 0;
}
