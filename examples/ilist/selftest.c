/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/ilist.h>

#define TEST_LOOP 10

struct test_node {
    struct bfdev_ilist_node ilist;
    unsigned long num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define ilist_to_test(ptr) \
    bfdev_ilist_entry(ptr, struct test_node, ilist)

static inline long
ilist_test_cmp(struct bfdev_ilist_node *nodea, struct bfdev_ilist_node *nodeb, const void *pdata)
{
    struct test_node *inodea = ilist_to_test(nodea);
    struct test_node *inodeb = ilist_to_test(nodeb);
    if (inodea->num == inodeb->num) return 0;
    return inodea->num < inodeb->num ? -1 : 1;
}

static int
bfdev_ilist_selftest(struct test_pdata *idata)
{
    unsigned int count, count2;

    BFDEV_ILIST_HEAD(test_head);

    for (count = 0; count < TEST_LOOP; ++count) {
        printf("ilist 'bfdev_ilist_add' test%u single\n", count);
        idata->nodes[count].num = count;
        bfdev_ilist_node_init(&idata->nodes[count].ilist);
        bfdev_ilist_add(&test_head, &idata->nodes[count].ilist,
                        ilist_test_cmp, NULL);
    }

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_ilist_del(&test_head, &idata->nodes[count].ilist);

    for (count = 0; count < TEST_LOOP / 2; ++count) {
        for (count2 = 0; count2 < 2; ++count2) {
            printf("ilist 'bfdev_ilist_add' test%u multi%u\n", count * 2 + count2, count2);
            idata->nodes[count].num = count;
            bfdev_ilist_node_init(&idata->nodes[count * 2 + count2].ilist);
            bfdev_ilist_add(&test_head, &idata->nodes[count * 2 + count2].ilist,
                            ilist_test_cmp, NULL);
        }
    }

    for (count = 0; count < TEST_LOOP; ++count)
        bfdev_ilist_del(&test_head, &idata->nodes[count].ilist);

    return 0;
}

int main(void)
{
    struct test_pdata *idata;
    int retval;

    idata = malloc(sizeof(*idata));
    if (!idata)
        return 1;

    retval = bfdev_ilist_selftest(idata);
    free(idata);

    return retval;
}
