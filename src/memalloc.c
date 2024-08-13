/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-memalloc"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/bits.h>
#include <bfdev/log.h>
#include <bfdev/memalloc.h>
#include <export.h>

static __bfdev_always_inline bool
pnode_get_used(bfdev_memalloc_chunk_t *node)
{
    return node->usize & BFDEV_BIT(0);
}

static __bfdev_always_inline size_t
pnode_get_size(bfdev_memalloc_chunk_t *node)
{
    return node->usize & BFDEV_BIT_HIGH_MASK(1);
}

static __bfdev_always_inline void
pnode_set_used(bfdev_memalloc_chunk_t *node, bool used)
{
    node->usize &= ~BFDEV_BIT(0);
    node->usize |= used;
}

static __bfdev_always_inline void
pnode_set_size(bfdev_memalloc_chunk_t *node, size_t size)
{
    node->usize &= ~BFDEV_BIT_HIGH_MASK(1);
    node->usize |= size & BFDEV_BIT_HIGH_MASK(1);
}

static __bfdev_always_inline void
pnode_set(bfdev_memalloc_chunk_t *node, size_t size, bool used)
{
    node->usize = (size & BFDEV_BIT_HIGH_MASK(1)) | used;
}

static inline bfdev_memalloc_chunk_t *
memalloc_check(void *block)
{
    bfdev_memalloc_chunk_t *node;

    /* Check whether it's a legal node */
    node = bfdev_container_of(block, bfdev_memalloc_chunk_t, data);

#ifdef BFDEV_DEBUG_MEMALLOC
    if (bfdev_unlikely(!bfdev_list_check_empty(&node->free))) {
        bfdev_log_crit("unlegal node at %p\n", block);
        return NULL;
    }
#endif

    return node;
}

export bfdev_memalloc_chunk_t *
bfdev_memalloc_first_fit(bfdev_memalloc_head_t *head, size_t size)
{
    bfdev_memalloc_chunk_t *node;

    bfdev_list_for_each_entry(node, &head->free_list, free) {
        if (pnode_get_size(node) >= size)
            return node;
    }

    return NULL;
}

export bfdev_memalloc_chunk_t *
bfdev_memalloc_best_fit(bfdev_memalloc_head_t *head, size_t size)
{
    bfdev_memalloc_chunk_t *best, *node;
    size_t walk, bsize;

    best = NULL;
    bsize = BFDEV_SIZE_MAX;

    bfdev_list_for_each_entry(node, &head->free_list, free) {
        walk = pnode_get_size(node);
        if (walk >= size) {
            if (walk == size)
                return node;
            else if (walk >= bsize)
                continue;
            best = node;
            bsize = walk;
        }
    }

    return best;
}

export bfdev_memalloc_chunk_t *
bfdev_memalloc_worst_fit(bfdev_memalloc_head_t *head, size_t size)
{
    bfdev_memalloc_chunk_t *worst, *node;
    size_t walk, bsize;

    worst = NULL;
    bsize = BFDEV_SIZE_MIN;

    bfdev_list_for_each_entry(node, &head->free_list, free) {
        walk = pnode_get_size(node);
        if (walk >= size) {
            if (walk == head->avail)
                return node;
            else if (walk <= bsize)
                continue;
            worst = node;
            bsize = walk;
        }
    }

    return worst;
}

export void *
bfdev_memalloc_alloc(bfdev_memalloc_head_t *head, size_t size)
{
    bfdev_memalloc_chunk_t *node, *free;
    size_t nsize, bsize, fsize;

    bfdev_align_high_adj(size, BFDEV_MEMALLOC_ALIGN);
    if (bfdev_unlikely(size > head->avail))
        return NULL;

    /* Get the free memory block */
    node = head->find(head, size);
    if (bfdev_unlikely(!node))
        return NULL;

    /* Adjust available size */
    nsize = pnode_get_size(node);
    head->avail -= nsize;

    bsize = nsize - size;
    if (bsize < sizeof(*node) + BFDEV_MEMALLOC_ALIGN)
        goto finish;

    /* Setup the new free node */
    free = (void *)node->data + size;
    fsize = bsize - sizeof(*free);

    pnode_set(free, fsize, false);
    head->avail += fsize;

    bfdev_list_add(&node->block, &free->block);
    bfdev_list_add(&head->free_list, &free->free);
    pnode_set_size(node, size);

finish:
    /* Set node used */
    pnode_set_used(node, true);
    bfdev_list_del_init(&node->free);
    return node->data;
}

export void *
bfdev_memalloc_realloc(bfdev_memalloc_head_t *head, void *block, size_t resize)
{
    bfdev_memalloc_chunk_t *node, *expand, *free;
    size_t origin, exsize, nsize, bsize, fsize;

    node = memalloc_check(block);
    if (bfdev_unlikely(!node))
        return NULL;

    bfdev_align_high_adj(resize, BFDEV_MEMALLOC_ALIGN);
    if (bfdev_unlikely(resize > head->avail))
        return NULL;

    origin = pnode_get_size(node);
    if (origin >= resize)
        return block;

    expand = bfdev_list_next_entry_or_null(node, &head->block_list, block);
    exsize = resize - origin;

    /* Check if direct expand is possible */
    if (!expand || pnode_get_used(expand) || (sizeof(*expand) +
        pnode_get_size(expand)) < exsize) {
        void *newblk;

        newblk = bfdev_memalloc_alloc(head, resize);
        if (bfdev_unlikely(!newblk))
            return NULL;

        bfport_memcpy(newblk, block, origin);
        bfdev_memalloc_free(head, block);

        return newblk;
    }

    /* Adjust available size */
    nsize = pnode_get_size(expand);
    head->avail -= nsize;

    bfdev_list_del(&expand->free);
    bfdev_list_del(&expand->block);

    /* Use all space of the next node */
    bsize = sizeof(*expand) + nsize - exsize;
    if (bsize < sizeof(*free) + BFDEV_MEMALLOC_ALIGN) {
        exsize = sizeof(*node) + nsize;
        resize = origin + exsize;
        goto finish;
    }

    /* Setup the new free node */
    free = (void *)expand + exsize;
    fsize = bsize - sizeof(*free);

    pnode_set(free, fsize, false);
    head->avail += fsize;

    bfdev_list_add(&node->block, &free->block);
    bfdev_list_add(&head->free_list, &free->free);

finish:
    pnode_set_size(node, resize);
    return block;
}

export void
bfdev_memalloc_free(bfdev_memalloc_head_t *head, void *block)
{
    bfdev_memalloc_chunk_t *side, *node;
    size_t nsize, fsize;

    node = memalloc_check(block);
    if (bfdev_unlikely(!node))
        return;

    /* Set node freed */
    pnode_set_used(node, false);
    bfdev_list_add(&head->free_list, &node->free);

    /* Adjust available size */
    nsize = pnode_get_size(node);
    head->avail += nsize;

    /* Merge next node */
    side = bfdev_list_next_entry_or_null(node, &head->block_list, block);
    if (side && !pnode_get_used(side)) {
        bfdev_list_del(&side->free);
        bfdev_list_del(&side->block);

        /* node size = this node + next node + next size */
        fsize = sizeof(*side) + pnode_get_size(side);
        pnode_set_size(node, pnode_get_size(node) + fsize);
        head->avail += sizeof(*side);
    }

    /* Merge prev node */
    side = bfdev_list_prev_entry_or_null(node, &head->block_list, block);
    if (side && !pnode_get_used(side)) {
        bfdev_list_del(&node->free);
        bfdev_list_del(&node->block);

        /* prev size = prev size + this node + this size */
        fsize = sizeof(*node) + pnode_get_size(node);
        pnode_set_size(side, pnode_get_size(side) + fsize);
        head->avail += sizeof(*node);
    }
}

export void
bfdev_memalloc_init(bfdev_memalloc_head_t *head, bfdev_memalloc_find_t find,
                    void *array, size_t size)
{
    bfdev_memalloc_chunk_t *node;

    bfdev_list_head_init(&head->block_list);
    bfdev_list_head_init(&head->free_list);
    head->find = find;

    node = array;
    pnode_set(node, size - sizeof(*node), false);
    head->avail = size - sizeof(*node);

    bfdev_list_add(&head->block_list, &node->block);
    bfdev_list_add(&head->free_list, &node->free);
}
