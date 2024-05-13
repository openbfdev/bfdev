/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "slist-iterator"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/macro.h>
#include <bfdev/slist.h>
#include <bfdev/log.h>
#include <testsuite.h>

#define TEST_LOOP 10

struct test_node {
    bfdev_slist_head_t list;
    unsigned int num;
};

struct test_struct {
    bfdev_slist_head_t head;
    struct test_node nodes[TEST_LOOP];
};

#define slist_to_test(ptr) \
    bfdev_slist_entry(ptr, struct test_node, list)

static void *
test_prepare(int argc, const char *argv[])
{
    struct test_struct *test;
    unsigned int count;

    test = malloc(sizeof(*test));
    if (!test)
        return NULL;

    bfdev_slist_head_init(&test->head);
    for (count = 0; count < TEST_LOOP; ++count) {
        test->nodes[count].num = TEST_LOOP - count;
        bfdev_slist_add(&test->head, &test->nodes[count].list);
    }

    return test;
}

static void
test_release(void *data)
{
    free(data);
}

TESTSUITE(
    "slist:for_each",
    test_prepare, test_release,
    "slist for each iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_slist_head_t *list, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_slist_for_each(list, &test->head) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_slist_for_each_continue(list) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each_continue' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_slist_for_each_from(list) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    bfdev_log_debug("'slist_for_each_continue' null test\n");
    bfdev_slist_for_each_continue(list)
        return -BFDEV_ENODATA;

    bfdev_log_debug("'slist_for_each_from' null test\n");
    bfdev_slist_for_each_from(list)
        return -BFDEV_ENODATA;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "slist:for_each_safe",
    test_prepare, test_release,
    "slist for each safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_slist_head_t *list, *nlist, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_slist_for_each_safe(list, nlist, &test->head) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_slist_for_each_continue_safe(list, nlist) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each_continue_safe' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_slist_for_each_from_safe(list, nlist) {
        node = slist_to_test(list);
        bfdev_log_debug("'for_each_from_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    bfdev_log_debug("'slist_for_each_continue_safe' null test\n");
    bfdev_slist_for_each_continue_safe(list, nlist)
        return -BFDEV_ENODATA;

    bfdev_log_debug("'slist_for_each_from_safe' null test\n");
    bfdev_slist_for_each_from_safe(list, nlist)
        return -BFDEV_ENODATA;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "slist:for_each_entry",
    test_prepare, test_release,
    "slist for each entry iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_slist_for_each_entry(node, &test->head, list) {
        bfdev_log_debug("'for_each_entry' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_slist_for_each_entry_continue(node, list) {
        bfdev_log_debug("'for_each_entry_continue' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_slist_for_each_entry_from(node, list) {
        bfdev_log_debug("'for_each_entry_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    bfdev_log_debug("'slist_for_each_entry_continue' null test\n");
    bfdev_slist_for_each_entry_continue(node, list)
        return -BFDEV_ENODATA;

    bfdev_log_debug("'slist_for_each_entry_from' null test\n");
    bfdev_slist_for_each_entry_from(node, list)
        return -BFDEV_ENODATA;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "slist:for_each_entry_safe",
    test_prepare, test_release,
    "slist for each entry safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *nnode, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_slist_for_each_entry_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'for_each_entry_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_slist_for_each_entry_continue_safe(node, nnode, list) {
        bfdev_log_debug("'for_each_entry_continue_safe' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_slist_for_each_entry_from_safe(node, nnode, list) {
        bfdev_log_debug("'for_each_entry_from_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    bfdev_log_debug("'slist_for_each_entry_continue_safe' null test\n");
    bfdev_slist_for_each_entry_continue_safe(node, nnode, list)
        return -BFDEV_ENODATA;

    bfdev_log_debug("'slist_for_each_entry_from_safe' null test\n");
    bfdev_slist_for_each_entry_from_safe(node, nnode, list)
        return -BFDEV_ENODATA;

    return -BFDEV_ENOERR;
}
