/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/skiplist.h>
#include <export.h>

static unsigned int
random_level(struct bfdev_skip_head *head)
{
    unsigned int level = 1;

    while (level < head->levels) {
        if (rand() > RAND_MAX >> 2)
            break;
        level++;
    }

    return level;
}

static struct bfdev_skip_node *
skipnode_find(struct bfdev_skip_head *head, const void *key,
              bfdev_skiplist_find_t find, unsigned int *plev)
{
    unsigned int level = head->curr;
    struct bfdev_list_head *list, *end;
    struct bfdev_skip_node *walk;
    long retval;

    if (unlikely(!level))
        return NULL;

    list = &head->nodes[level - 1];
    end = &head->nodes[level - 1];

    for (; level--; --list, --end) {
        bfdev_list_for_each_continue(list, end) {
            walk = bfdev_list_entry(list, struct bfdev_skip_node, list[level]);
            retval = find(walk->pdata, key);
            if (retval >= 0) {
                end = list;
                break;
            }
        }

        if (retval == 0) {
            if (plev)
                *plev = level + 1;
            return walk;
        }

        list = end->prev;
    }

    return NULL;
}

export int
bfdev_skiplist_insert(struct bfdev_skip_head *head, void *data,
                      bfdev_skiplist_cmp_t cmp)
{
    const struct bfdev_alloc *alloc = head->alloc;
    struct bfdev_list_head *list, *end;
    struct bfdev_skip_node *walk, *node;
    unsigned int level, count;
    long retval;

    level = random_level(head);
    bfdev_max_adj(head->curr, level);

    node = bfdev_malloc(alloc, sizeof(*node) + sizeof(*node->list) * level);
    if (unlikely(!node))
        return -BFDEV_ENOMEM;

    node->pdata = data;
    list = &head->nodes[head->curr - 1];
    end = &head->nodes[head->curr - 1];

    for (count = head->curr; count--; --list, --end) {
        bfdev_list_for_each_continue(list, end) {
            walk = bfdev_list_entry(list, struct bfdev_skip_node, list[count]);
            retval = cmp(walk->pdata, data);
            if (retval >= 0) {
                end = list;
                break;
            }
        }

        list = end->prev;
        if (count < level)
            bfdev_list_add(list, &node->list[count]);
    }

    return 0;
}

export void
bfdev_skiplist_delete(struct bfdev_skip_head *head, void *key,
                      bfdev_skiplist_find_t find)
{
    const struct bfdev_alloc *alloc = head->alloc;
    struct bfdev_skip_node *node;
    unsigned int level;

    node = skipnode_find(head, key, find, &level);
    if (unlikely(!node))
        return;

    while (level--) {
        bfdev_list_del(&node->list[level]);
        if (bfdev_list_check_empty(&head->nodes[level]))
            head->curr = level;
    }

    bfdev_free(alloc, node);
}

export struct bfdev_skip_node *
bfdev_skiplist_find(struct bfdev_skip_head *head, void *key,
                    bfdev_skiplist_find_t find)
{
    struct bfdev_skip_node *node;
    node = skipnode_find(head, key, find, NULL);
    return node;
}

static void
bfdev_skiplist_release(struct bfdev_skip_head *head,
                       bfdev_skiplist_release_t relse)
{
    const struct bfdev_alloc *alloc = head->alloc;
    struct bfdev_skip_node *node, *tmp;

    bfdev_list_for_each_entry_safe(node, tmp, head->nodes, list[0]) {
        if (relse)
            relse(node->pdata);
        bfdev_free(alloc, node);
    }
}

export void
bfdev_skiplist_reset(struct bfdev_skip_head *head,
                     bfdev_skiplist_release_t relse)
{
    unsigned int count;

    bfdev_skiplist_release(head, relse);
    for (count = 0; count < head->levels; ++count)
        bfdev_list_head_init(&head->nodes[count]);

    head->curr = 0;
}

export void
bfdev_skiplist_destroy(struct bfdev_skip_head *head,
                       bfdev_skiplist_release_t relse)
{
    const struct bfdev_alloc *alloc = head->alloc;

    bfdev_skiplist_release(head, relse);
    bfdev_free(alloc, head);
}

export struct bfdev_skip_head *
bfdev_skiplist_create(const struct bfdev_alloc *alloc,
                      unsigned int levels)
{
    struct bfdev_skip_head *head;
    unsigned int count;

    if (unlikely(!levels))
        return NULL;

    head = bfdev_malloc(alloc, sizeof(*head) + sizeof(*head->nodes) * levels);
    if (unlikely(!head))
        return NULL;

    for (count = 0; count < levels; ++count)
        bfdev_list_head_init(&head->nodes[count]);

    head->alloc = alloc;
    head->levels = levels;
    head->curr = 0;

    return head;
}
