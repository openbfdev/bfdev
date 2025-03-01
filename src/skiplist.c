/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/skiplist.h>
#include <export.h>

static unsigned int
random_level(bfdev_skip_head_t *head)
{
    unsigned int level;

    level = 1;
    while (level < head->levels) {
        if (bfport_rand() > RAND_MAX >> 2)
            break;
        level++;
    }

    return level;
}

static bfdev_skip_node_t *
skipnode_find(bfdev_skip_head_t *head, bfdev_find_t find,
              void *pdata, unsigned int *plev)
{
    bfdev_list_head_t *list, *end;
    bfdev_skip_node_t *walk;
    unsigned int level;
    long retval;

    level = head->curr;
    if (bfdev_unlikely(!level))
        return NULL;

    list = &head->nodes[level - 1];
    end = list;

    for (; level--; --list, --end) {
        retval = BFDEV_LT;
        bfdev_list_for_each_continue(list, end) {
            walk = bfdev_list_entry(list, bfdev_skip_node_t, list[level]);
            retval = find(walk->key, pdata);
            if (retval >= 0) {
                end = list;
                break;
            }
        }

        if (!retval) {
            if (plev)
                *plev = level + 1;
            return walk;
        }

        list = end->prev;
    }

    return NULL;
}

export bfdev_skip_node_t *
bfdev_skiplist_find(bfdev_skip_head_t *head, bfdev_find_t find, void *pdata)
{
    return skipnode_find(head, find, pdata, NULL);
}

export int
bfdev_skiplist_insert(bfdev_skip_head_t *head, void *key,
                      bfdev_cmp_t cmp, void *pdata)
{
    const bfdev_alloc_t *alloc;
    bfdev_list_head_t *list, *end;
    bfdev_skip_node_t *walk, *node;
    unsigned int level, count;
    long retval;

    level = random_level(head);
    bfdev_max_adj(head->curr, level);

    alloc = head->alloc;
    node = bfdev_malloc(alloc, sizeof(*node) + sizeof(*node->list) * level);
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOMEM;

    node->key = key;
    list = &head->nodes[head->curr - 1];
    end = &head->nodes[head->curr - 1];

    for (count = head->curr; count--; --list, --end) {
        bfdev_list_for_each_continue(list, end) {
            walk = bfdev_list_entry(list, bfdev_skip_node_t, list[count]);
            retval = cmp(walk->key, key, pdata);
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

export int
bfdev_skiplist_delete(bfdev_skip_head_t *head, bfdev_find_t find, void *pdata)
{
    const bfdev_alloc_t *alloc;
    bfdev_skip_node_t *node;
    unsigned int level;

    alloc = head->alloc;
    node = skipnode_find(head, find, pdata, &level);
    if (bfdev_unlikely(!node))
        return -BFDEV_ENOENT;

    while (level--) {
        bfdev_list_del(&node->list[level]);
        if (bfdev_list_check_empty(&head->nodes[level]))
            head->curr = level;
    }

    bfdev_free(alloc, node);

    return -BFDEV_ENOERR;
}

static void
skiplist_release(bfdev_skip_head_t *head, bfdev_release_t release, void *pdata)
{
    const bfdev_alloc_t *alloc;
    bfdev_skip_node_t *node, *tmp;

    alloc = head->alloc;
    bfdev_list_for_each_entry_safe(node, tmp, head->nodes, list[0]) {
        if (release)
            release(node->key, pdata);
        bfdev_free(alloc, node);
    }
}

export void
bfdev_skiplist_reset(bfdev_skip_head_t *head, bfdev_release_t release,
                     void *pdata)
{
    unsigned int count;

    skiplist_release(head, release, pdata);
    for (count = 0; count < head->levels; ++count)
        bfdev_list_head_init(&head->nodes[count]);

    head->curr = 0;
}

export bfdev_skip_head_t *
bfdev_skiplist_create(const bfdev_alloc_t *alloc, unsigned int levels)
{
    bfdev_skip_head_t *head;
    unsigned int count;

    if (bfdev_unlikely(!levels))
        return NULL;

    head = bfdev_malloc(alloc, sizeof(*head) + sizeof(*head->nodes) * levels);
    if (bfdev_unlikely(!head))
        return NULL;

    for (count = 0; count < levels; ++count)
        bfdev_list_head_init(&head->nodes[count]);

    head->alloc = alloc;
    head->levels = levels;
    head->curr = 0;

    return head;
}

export void
bfdev_skiplist_destroy(bfdev_skip_head_t *head, bfdev_release_t release,
                       void *pdata)
{
    const bfdev_alloc_t *alloc;

    alloc = head->alloc;
    skiplist_release(head, release, pdata);
    bfdev_free(alloc, head);
}
