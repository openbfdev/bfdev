/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "respool-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdlib.h>
#include <bfdev/log.h>
#include <bfdev/respool.h>

#define TEST_LOOP 16
#define TEST_SIZE 4096

struct test_struct {
    bfdev_respool_node_t node;
    unsigned int count;
    void *data;
};

static void
cleanup(bfdev_respool_node_t *node, void *pdata)
{
    struct test_struct *test;

    test = bfdev_container_of(node, struct test_struct, node);
    bfdev_log_info("release %d: %p\n", test->count, test->data);

    free(test->data);
    free(test);
}

int
main(int argc, char *argv[])
{
    BFDEV_DEFINE_RESPOOL(respool);
    struct test_struct *test;
    unsigned int count;

    for (count = 0; count < TEST_LOOP; ++count) {
        test = malloc(sizeof(*test));
        if (!test)
            return 1;

        test->data = malloc(TEST_SIZE);
        if (!test->data)
            return 1;

        test->count = count;
        test->node.release = cleanup;

        bfdev_log_info("insert %d: %p\n", count, test->data);
        bfdev_respool_insert(&respool, &test->node);
    }

    bfdev_respool_release_all(&respool, NULL);

    return 0;
}
