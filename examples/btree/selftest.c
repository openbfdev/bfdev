/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Fredrik Anderson <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <bfdev/btree.h>
#include <bfdev/list.h>

#define TEST_LOOP 100

struct test_node {
    bfdev_list_head_t list;
    union {
        uintptr_t key;
        char uuid[11];
    };
};

static int
test_clash(bfdev_btree_root_t *root, void *value, void *clash)
{
    struct test_node *vnode = value;
    struct test_node *cnode = clash;
    bfdev_list_add(&vnode->list, &cnode->list);
    return 0;
}

static void *
test_remove(bfdev_btree_root_t *root, void *value)
{
    struct test_node *vnode = value;
    struct test_node *remove;

    if (bfdev_list_check_empty(&vnode->list))
        return NULL;

    remove = bfdev_list_first_entry(&vnode->list, struct test_node, list);
    bfdev_list_del(&remove->list);

    return remove;
}

static long
test_strfind(bfdev_btree_root_t *root, uintptr_t *node, uintptr_t *key)
{
    const char *nstring = (void *)*node ?: "";
    const char *kstring = (void *)*key ?: "";
    return strcmp(nstring, kstring);
}

static const bfdev_btree_ops_t
test_value_ops = {
    .alloc = bfdev_btree_alloc,
    .free = bfdev_btree_free,
    .find = bfdev_btree_key_find,
    .clash = test_clash,
    .remove = test_remove,

};

static const bfdev_btree_ops_t
test_string_ops = {
    .alloc = bfdev_btree_alloc,
    .free = bfdev_btree_free,
    .find = test_strfind,
    .clash = test_clash,
    .remove = test_remove,
};

static int
test_testing(struct test_node *nodes)
{
    struct test_node *lookup;
    unsigned int count;
    uintptr_t insert;
    void *value;
    int retval;

    BFDEV_BTREE_ROOT(
        root32, &bfdev_btree_layout32,
        &test_value_ops, NULL
    );

    srand(time(NULL));
    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_list_head_init(&nodes[count].list);
        nodes[count].key = (uintptr_t)rand();
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        retval = bfdev_btree_insert(&root32, &nodes[count].key, &nodes[count]);
        printf("btree random insert test%d: %#010lx retval %d\n", count,
                (unsigned long)nodes[count].key, retval);
        if (retval)
            return retval;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        lookup = bfdev_btree_lookup(&root32, &nodes[count].key);
        printf("btree random lookup test%d: ", count);
        if (!lookup || (lookup != &nodes[count] && bfdev_list_check_empty(&lookup->list))) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    bfdev_btree_for_each(&root32, &insert, value) {
        printf("btree random for each: %#010lx = %p\n",
               (unsigned long)insert, value);
    }

    bfdev_btree_for_each_reverse(&root32, &insert, value) {
        printf("btree random for each reverse: %#010lx = %p\n",
               (unsigned long)insert, value);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        lookup = bfdev_btree_remove(&root32, &nodes[count].key);
        printf("btree random remove test%d\n", count);
    }

    bfdev_btree_release(&root32, NULL, NULL);

    BFDEV_BTREE_ROOT(
        rootstr, &bfdev_btree_layout32,
        &test_string_ops, NULL
    );

    for (count = 0; count < TEST_LOOP; ++count) {
        bfdev_list_head_init(&nodes[count].list);
        snprintf(nodes[count].uuid, sizeof(nodes->uuid),
                 "%#010x", (unsigned int)rand());
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        insert = (uintptr_t)&nodes[count].uuid;
        retval = bfdev_btree_insert(&rootstr, &insert, &nodes[count]);
        printf("btree string insert test%d: %s retval %d\n", count,
                nodes[count].uuid, retval);
        if (retval)
            return retval;
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        insert = (uintptr_t)&nodes[count].uuid;
        lookup = bfdev_btree_lookup(&rootstr, &insert);
        printf("btree string lookup test%d: ", count);
        if (!lookup || (lookup != &nodes[count] && bfdev_list_check_empty(&lookup->list))) {
            printf("failed\n");
            return 1;
        }
        printf("pass\n");
    }

    bfdev_btree_for_each(&rootstr, &insert, value) {
        printf("btree string for each: %s = %p\n",
                (char *)insert, value);
    }

    bfdev_btree_for_each_reverse(&rootstr, &insert, value) {
        printf("btree string for each reverse: %s = %p\n",
                (char *)insert, value);
    }

    for (count = 0; count < TEST_LOOP; ++count) {
        insert = (uintptr_t)&nodes[count].uuid;
        lookup = bfdev_btree_remove(&rootstr, &insert);
        printf("btree string remove test%d\n", count);
    }

    bfdev_btree_release(&rootstr, NULL, NULL);

    return 0;
}

int
main(int argc, const char *argv[])
{
    struct test_node *bdata;
    int retval;

    bdata = malloc(sizeof(*bdata) * TEST_LOOP);
    if (!bdata)
        return 1;

    retval = test_testing(bdata);
    free(bdata);

    return retval;
}
