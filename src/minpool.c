/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/bits.h>
#include <bfdev/minpool.h>
#include <export.h>

struct bfdev_minpool_node {
    struct bfdev_list_head block;
    struct bfdev_list_head free;
    size_t usize;
    char data[0];
};

static __bfdev_always_inline bool
minnode_get_used(struct bfdev_minpool_node *node)
{
    return node->usize & BFDEV_BIT(0);
}

static __bfdev_always_inline size_t
minnode_get_size(struct bfdev_minpool_node *node)
{
    return node->usize & BFDEV_BIT_HIGH_MASK(1);
}

static __bfdev_always_inline void
minnode_set_used(struct bfdev_minpool_node *node, bool used)
{
    node->usize &= ~BFDEV_BIT(0);
    node->usize |= used;
}

static __bfdev_always_inline void
minnode_set_size(struct bfdev_minpool_node *node, size_t size)
{
    node->usize &= ~BFDEV_BIT_HIGH_MASK(0);
    node->usize |= size & BFDEV_BIT_HIGH_MASK(1);
}

static __bfdev_always_inline void
minnode_set(struct bfdev_minpool_node *node, size_t size, bool used)
{
    node->usize = (size & BFDEV_BIT_HIGH_MASK(1)) | used;
}

/**
 * bfdev_minpool_find - Get first qualified node in mempool.
 * @head: Minimum mempool to get node.
 * @size: Node minimum size to get.
 */
static struct bfdev_minpool_node *
minpool_find(struct bfdev_minpool_head *head, size_t size)
{
    struct bfdev_minpool_node *node;

    bfdev_list_for_each_entry(node, &head->free_list, free) {
        if (minnode_get_size(node) >= size)
            return node;
    }

    return NULL;
}

static inline struct bfdev_minpool_node *
minpool_check(void *block)
{
    struct bfdev_minpool_node *node;

    /* Check whether it's a legal node */
    node = container_of(block, struct bfdev_minpool_node, data);
    if (unlikely(!bfdev_list_check_empty(&node->free)))
        return NULL;

    return node;
}

export void *
bfdev_minpool_alloc(struct bfdev_minpool_head *head, size_t size)
{
    struct bfdev_minpool_node *node;
    size_t fsize;

    if (unlikely(!size))
        return NULL;

    bfdev_align_high_adj(size, BFDEV_MINPOOL_ALIGN);
    if (unlikely(size > head->avail))
        return NULL;

    /* Get the free memory block */
    node = minpool_find(head, size);
    if (unlikely(!node))
        return NULL;

    fsize = minnode_get_size(node);
    if (fsize - size >= sizeof(*node) + BFDEV_MINPOOL_ALIGN) {
        struct bfdev_minpool_node *free;

        /* Setup the new free block */
        free = (void *)node->data + size;
        minnode_set(free, fsize - size - sizeof(*free), false);

        bfdev_list_add(&head->free_list, &free->free);
        bfdev_list_add(&node->block, &free->block);

        minnode_set_size(node, size);
        head->avail -= sizeof(*free);
        fsize = size;
    }

    /* Set node used */
    minnode_set_used(node, true);
    bfdev_list_del_init(&node->free);

    /* Adjust heap available size */
    head->avail -= fsize;
    return node->data;
}

export void
bfdev_minpool_free(struct bfdev_minpool_head *head, void *block)
{
    struct bfdev_minpool_node *other, *node;
    size_t fsize;

    if (unlikely(!block))
        return;

    node = minpool_check(block);
    if (unlikely(!node))
        return;

    /* Set node freed */
    minnode_set_used(node, false);
    bfdev_list_add(&head->free_list, &node->free);

    fsize = minnode_get_size(node);
    head->avail += fsize;

    /* Merge next node */
    other = bfdev_list_next_entry_or_null(node, &head->block_list, block);
    if (other && !minnode_get_used(other)) {
        /* node size = this node + next node + next size */
        minnode_set_size(node, fsize + sizeof(*other) + minnode_get_size(other));
        bfdev_list_del(&other->block);
        bfdev_list_del(&other->free);
        head->avail += sizeof(*other);
    }

    /* Merge prev node */
    other = bfdev_list_prev_entry_or_null(node, &head->block_list, block);
    if (other && !minnode_get_used(other)) {
        /* prev size = prev size + this node + this size */
        minnode_set_size(other, minnode_get_size(other) + sizeof(*node) + fsize);
        bfdev_list_del(&node->block);
        bfdev_list_del(&node->free);
        head->avail += sizeof(*node);
    }
}

export void *
bfdev_minpool_realloc(struct bfdev_minpool_head *head, void *block, size_t resize)
{
    struct bfdev_minpool_node *node, *expand;
    size_t origin, exsize, fsize;

    if (unlikely(!block))
        return bfdev_minpool_alloc(head, resize);

    if (unlikely(!resize)) {
        bfdev_minpool_free(head, block);
        return NULL;
    }

    node = minpool_check(block);
    if (unlikely(!node))
        return NULL;

    bfdev_align_high_adj(resize, BFDEV_MINPOOL_ALIGN);
    if (unlikely(resize > head->avail))
        return NULL;

    origin = minnode_get_size(node);
    if (origin >= resize)
        return block;

    expand = bfdev_list_next_entry_or_null(node, &head->block_list, block);
    if (!expand || minnode_get_used(expand) || sizeof(*expand) +
        minnode_get_size(expand) < (exsize = resize - origin)) {
        void *newblk;

        newblk = bfdev_minpool_alloc(head, resize);
        if (unlikely(!newblk))
            return NULL;

        memcpy(newblk, block, origin);
        bfdev_minpool_free(head, block);

        return newblk;
    }

    fsize = minnode_get_size(expand);
    bfdev_list_del(&expand->block);
    bfdev_list_del(&expand->free);

    if (fsize - exsize < sizeof(*node) + BFDEV_MINPOOL_ALIGN) {
        /* Use all space of the next node */
        exsize = sizeof(*node) + fsize;
        resize = origin + exsize;
    } else {
        /* Detach free node */
        expand = (void *)expand + exsize;
        minnode_set(expand, fsize - exsize - sizeof(*node), false);
        bfdev_list_add(&node->block, &expand->block);
        bfdev_list_add(&head->free_list, &expand->free);
    }

    minnode_set_size(node, resize);
    head->avail -= exsize;

    return block;
}

export void
bfdev_minpool_setup(struct bfdev_minpool_head *head, void *array, size_t size)
{
    struct bfdev_minpool_node *node = array;

    bfdev_list_head_init(&head->block_list);
    bfdev_list_head_init(&head->free_list);

    minnode_set_used(node, false);
    minnode_set_size(node, size - sizeof(*node));
    head->avail = size - sizeof(*node);

    bfdev_list_add(&head->block_list, &node->block);
    bfdev_list_add(&head->free_list, &node->free);
}
