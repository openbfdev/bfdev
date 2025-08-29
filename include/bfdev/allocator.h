/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ALLOCATOR_H_
#define _BFDEV_ALLOCATOR_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/guards.h>
#include <bfdev/errptr.h>

BFDEV_BEGIN_DECLS

typedef struct bfdev_alloc bfdev_alloc_t;
typedef struct bfdev_alloc_ops bfdev_alloc_ops_t;

struct bfdev_alloc {
    const bfdev_alloc_ops_t *ops;
    void *pdata;
};

struct bfdev_alloc_ops {
    bfdev_malloc_t alloc;
    bfdev_malloc_t zalloc;
    bfdev_realloc_t realloc;
    bfdev_free_t free;
};

#define BFDEV_ALLOC_OPS_STATIC(ALLOC, ZALLOC, REALLOC, FREE) { \
    .alloc = (ALLOC), .zalloc = (ZALLOC), \
    .realloc = (REALLOC), .free = (FREE), \
}

#define BFDEV_ALLOC_OPS_INIT(alloc, zalloc, realloc, free) \
    (bfdev_alloc_ops_t) BFDEV_ALLOC_OPS_STATIC(alloc, zalloc, realloc, free)

#define BFDEV_DEFINE_ALLOC_OPS(name, alloc, zalloc, realloc, free) \
    bfdev_alloc_ops_t name = BFDEV_ALLOC_OPS_INIT(alloc, zalloc, realloc, free)

#define BFDEV_ALLOC_STATIC(OPS, PDATA) { \
    .ops = (OPS), .pdata = (PDATA), \
}

#define BFDEV_ALLOC_INIT(ops, pdata) \
    (bfdev_alloc_t) BFDEV_ALLOC_STATIC(ops, pdata)

#define BFDEV_DEFINE_ALLOC(name, ops, pdata) \
    bfdev_alloc_t name = BFDEV_ALLOC_INIT(ops, pdata)

extern bfdev_alloc_t
bfdev_alloc_default;

extern bfdev_alloc_ops_t
bfdev_alloc_default_ops;

/**
 * bfdev_alloc_init() - initializes the allocator structure.
 * @alloc: allocator which will be initialized.
 * @ops: operations for the allocator.
 */
static inline void
bfdev_alloc_init(bfdev_alloc_t *alloc, const bfdev_alloc_ops_t *ops,
                 void *pdata)
{
    *alloc = BFDEV_ALLOC_INIT(ops, pdata);
}

static inline void
bfdev_alloc_ops_init(bfdev_alloc_ops_t *ops,
                     bfdev_malloc_t alloc, bfdev_malloc_t zalloc,
                     bfdev_realloc_t realloc, bfdev_free_t free)
{
    *ops = BFDEV_ALLOC_OPS_INIT(alloc, zalloc, realloc, free);
}

extern __bfdev_malloc void *
bfdev_malloc(const bfdev_alloc_t *alloc, bfdev_size_t size);

extern __bfdev_malloc void *
bfdev_zalloc(const bfdev_alloc_t *alloc, bfdev_size_t size);

extern __bfdev_malloc void *
bfdev_realloc(const bfdev_alloc_t *alloc, const void *block,
              bfdev_size_t resize);

extern void
bfdev_free(const bfdev_alloc_t *alloc, const void *block);

/**
 * bfdev_malloc_array - allocate memory for an array.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_malloc_array(const bfdev_alloc_t *alloc,
                   bfdev_size_t nr, bfdev_size_t size)
{
    return bfdev_malloc(alloc, size * nr);
}

/**
 * bfdev_zalloc_array - zero allocate memory for an array.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_zalloc_array(const bfdev_alloc_t *alloc,
                   bfdev_size_t nr, bfdev_size_t size)
{
    return bfdev_zalloc(alloc, size * nr);
}

/**
 * bfdev_realloc_array - reallocate memory for an array.
 * @block: number of elements.
 * @nr: number of elements.
 * @size: single element size.
 */
static __bfdev_always_inline __bfdev_malloc void *
bfdev_realloc_array(const bfdev_alloc_t *alloc,
                    void *block, bfdev_size_t nr, bfdev_size_t size)
{
    return bfdev_realloc(alloc, block, size * nr);
}

BFDEV_DEFINE_CLEAN(bfdev_free, void *,
    if (!BFDEV_IS_INVAL(_T))
        bfdev_free(BFDEV_NULL, _T);
)

BFDEV_DEFINE_CLASS(bfdev_malloc, void *,
    bfdev_malloc(BFDEV_NULL, size),
    bfdev_free(BFDEV_NULL, _T),
    bfdev_size_t size
)

BFDEV_DEFINE_CLASS(bfdev_zalloc, void *,
    bfdev_zalloc(BFDEV_NULL, size),
    bfdev_free(BFDEV_NULL, _T),
    bfdev_size_t size
)

BFDEV_END_DECLS

#endif /* _BFDEV_ALLOCATOR_H_ */
