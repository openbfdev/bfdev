/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021-2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bfdev.h>
#include <bfdev/rbtree.h>
#include <errno.h>

#define TEST_LEN 10
static RB_ROOT(simple_root);

struct simple_node {
    struct rb_node rb;
    unsigned long data;
};

#define rb_to_demo(node) \
    rb_entry_safe(node, struct simple_node, rb)

static long demo_cmp(const struct rb_node *a, const struct rb_node *b)
{
    struct simple_node *demo_a = rb_to_demo(a);
    struct simple_node *demo_b = rb_to_demo(b);
    return demo_a->data - demo_b->data;
}

int main(void)
{
    struct simple_node *node, *tmp;
    unsigned int count;

    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if (!node) {
            printf("Abort.\n");
            return -BFDEV_ENOMEM;
        }

        node->data = ((unsigned long)rand() << 32) | rand();
        rb_insert(&simple_root, &node->rb, demo_cmp);
    }

    printf("Middle Iteration:\n");
    rb_for_each_entry(node, &simple_root, rb)
        printf("  0x%16lx\n", node->data);

    printf("Postorder Iteration:\n");
    rb_post_for_each_entry(node, &simple_root, rb)
        printf("  0x%16lx\n", node->data);

    rb_post_for_each_entry_safe(node, tmp, &simple_root, rb)
        free(node);

    printf("Done.\n");
    return 0;
}
