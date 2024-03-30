/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "list-iterator"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/macro.h>
#include <bfdev/list.h>
#include <bfdev/log.h>
#include <testsuite.h>

#define TEST_LOOP 10

struct test_node {
    bfdev_list_head_t list;
    unsigned int num;
};

struct test_struct {
    bfdev_list_head_t head;
    struct test_node nodes[TEST_LOOP];
};

#define list_to_test(ptr) \
    bfdev_list_entry(ptr, struct test_node, list)

static void *
test_prepare(int argc, const char *argv[])
{
    struct test_struct *test;
    unsigned int count;

    test = malloc(sizeof(*test));
    if (!test)
        return NULL;

    bfdev_list_head_init(&test->head);
    for (count = 0; count < TEST_LOOP; ++count) {
        test->nodes[count].num = count;
        (count % 1 ? bfdev_list_add_prev : bfdev_list_add)
            (&test->head, &test->nodes[count].list);
    }

    return test;
}

static void
test_release(void *data)
{
    free(data);
}

TESTSUITE(
    "list:for_each",
    test_prepare, test_release,
    "list for each iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_list_head_t *list, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_continue(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_continue' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_list_for_each_from(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_reverse",
    test_prepare, test_release,
    "list for each reverse iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_list_head_t *list, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_reverse(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_reverse_continue(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse_continue' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_list_for_each_reverse_from(list, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_safe",
    test_prepare, test_release,
    "list for each safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_list_head_t *list, *nlist, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_continue_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_continue_safe' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_list_for_each_from_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_from_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_reverse_safe",
    test_prepare, test_release,
    "list for each reverse safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node;
    bfdev_list_head_t *list, *nlist, *tlist;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_reverse_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_reverse_continue_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse_continue_safe' test: %u\n", node->num);
        count++;
    }

    list = tlist;
    bfdev_list_for_each_reverse_from_safe(list, nlist, &test->head) {
        node = list_to_test(list);
        bfdev_log_debug("'list_for_each_reverse_from_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_entry",
    test_prepare, test_release,
    "list for each entry iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_entry(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_continue(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_continue' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_list_for_each_entry_from(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_entry_reverse",
    test_prepare, test_release,
    "list for each entry reverse iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_entry_reverse(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_reverse_continue(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse_continue' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_list_for_each_entry_reverse_from(node, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse_from' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_entry_safe",
    test_prepare, test_release,
    "list for each entry safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *nnode, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_entry_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_continue_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_continue_safe' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_list_for_each_entry_from_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_form_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}

TESTSUITE(
    "list:for_each_entry_reverse_safe",
    test_prepare, test_release,
    "list for each entry reverse safe iterator test"
) {
    struct test_struct *test;
    struct test_node *node, *nnode, *tnode;
    unsigned int count;

    test = data;
    count = 0;

    bfdev_list_for_each_entry_reverse_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse_safe' test: %u\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_reverse_continue_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse_continue_safe' test: %u\n", node->num);
        count++;
    }

    node = tnode;
    bfdev_list_for_each_entry_reverse_from_safe(node, nnode, &test->head, list) {
        bfdev_log_debug("'list_for_each_entry_reverse_form_safe' test: %u\n", node->num);
        count++;
    }

    if (count != TEST_LOOP + (TEST_LOOP / 2))
        return -BFDEV_EFAULT;

    return -BFDEV_ENOERR;
}
