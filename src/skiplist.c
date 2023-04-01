/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bfdev.h>
#include <bfdev/skiplist.h>

static unsigned int random_level(struct skip_head *head)
{
    unsigned int level = 1;

    while (level < head->levels) {
        if (rand() > RAND_MAX >> 2)
            break;
        level++;
    }

    return level;
}

static struct skip_node *
skipnode_find(struct skip_head *head, const void *key,
              skiplist_find_t find, unsigned int *plev)
{
    unsigned int level = head->curr;
    struct list_head *list, *end;
    struct skip_node *walk;
    long retval;

    if (unlikely(!level))
        return NULL;

    list = &head->nodes[level - 1];
    end = &head->nodes[level - 1];

    for (; level--; --list, --end) {
        list_for_each_continue(list, end) {
            walk = list_entry(list, struct skip_node, list[level]);
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

int skiplist_insert(struct skip_head *head, void *data,
                    skiplist_cmp_t cmp)
{
    struct list_head *list, *end;
    struct skip_node *walk, *node;
    unsigned int level, count;
    long retval;

    level = random_level(head);
    max_adj(head->curr, level);

    node = malloc(sizeof(*node) + sizeof(*node->list) * level);
    if (unlikely(!node))
        return -BFDEV_ENOMEM;

    node->pdata = data;
    list = &head->nodes[head->curr - 1];
    end = &head->nodes[head->curr - 1];

    for (count = head->curr; count--; --list, --end) {
        list_for_each_continue(list, end) {
            walk = list_entry(list, struct skip_node, list[count]);
            retval = cmp(walk->pdata, data);
            if (retval >= 0) {
                end = list;
                break;
            }
        }

        list = end->prev;
        if (count < level)
            list_add(list, &node->list[count]);
    }

    return 0;
}

void skiplist_delete(struct skip_head *head, void *key, skiplist_find_t find)
{
    struct skip_node *node;
    unsigned int level;

    node = skipnode_find(head, key, find, &level);
    if (unlikely(!node))
        return;

    while (level--) {
        list_del(&node->list[level]);
        if (list_check_empty(&head->nodes[level]))
            head->curr = level;
    }

    free(node);
}

void *skiplist_find(struct skip_head *head, void *key, skiplist_find_t find)
{
    struct skip_node *node;
    node = skipnode_find(head, key, find, NULL);
    return node ? node->pdata : NULL;
}

static void skiplist_release(struct skip_head *head, skiplist_release_t relse)
{
    struct skip_node *node, *tmp;
    list_for_each_entry_safe(node, tmp, head->nodes, list[0]) {
        if (relse)
            relse(node->pdata);
        free(node);
    }
}

void skiplist_reset(struct skip_head *head, skiplist_release_t relse)
{
    unsigned int count;

    skiplist_release(head, relse);
    for (count = 0; count < head->levels; ++count)
        list_head_init(&head->nodes[count]);

    head->curr = 0;
}

void skiplist_destroy(struct skip_head *head, skiplist_release_t relse)
{
    skiplist_release(head, relse);
    free(head);
}

struct skip_head *skiplist_create(unsigned int levels)
{
    struct skip_head *head;
    unsigned int count;

    if (unlikely(!levels))
        return NULL;

    head = malloc(sizeof(*head) + sizeof(*head->nodes) * levels);
    if (unlikely(!head))
        return NULL;

    for (count = 0; count < levels; ++count)
        list_head_init(&head->nodes[count]);

    head->levels = levels;
    head->curr = 0;

    return head;
}
