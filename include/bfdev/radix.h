/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_RADIX_H_
#define _BFDEV_RADIX_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/allocator.h>
#include <bfdev/log2.h>
#include <bfdev/bitmap.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_RADIX_SHIFT
# define BFDEV_RADIX_SHIFT 8
#endif

#define BFDEV_RADIX_BLOCK BFDEV_BIT(BFDEV_RADIX_SHIFT)
#define BFDEV_RADIX_ARY (BFDEV_RADIX_BLOCK / sizeof(bfdev_radix_node_t *))

typedef struct bfdev_radix_root bfdev_radix_root_t;
typedef struct bfdev_radix_node bfdev_radix_node_t;

struct bfdev_radix_root {
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node;
    unsigned int level;
};

struct bfdev_radix_node {
    union {
        struct { /* branch */
            bfdev_radix_node_t *child[BFDEV_RADIX_ARY];
            unsigned int refcount;
        };
        struct { /* leaf */
            uint8_t block[BFDEV_RADIX_BLOCK];
            BFDEV_DEFINE_BITMAP(bitmap, BFDEV_RADIX_BLOCK);
        };
    };
};

#define BFDEV_RADIX_CHECK(type) \
    (sizeof(type) > BFDEV_RADIX_BLOCK ? -1 : 1)

#define BFDEV_GENERIC_RADIX(datatype)               \
    union {                                         \
        bfdev_radix_root_t tree;                    \
        datatype data;                              \
        char check[BFDEV_RADIX_CHECK(datatype)];    \
    }

#define BFDEV_RADIX_STATIC(ALLOC) { \
    .tree = {.alloc = (ALLOC)}, \
}

#define BFDEV_RADIX_INIT(root, alloc) \
    (typeof(*root)) BFDEV_RADIX_STATIC(alloc)

#define BFDEV_DECLARE_RADIX(name, type) \
    BFDEV_GENERIC_RADIX(type) name

#define BFDEV_DEFINE_RADIX(name, type, alloc) \
    BFDEV_DECLARE_RADIX(name, type) = BFDEV_RADIX_INIT(&name, alloc)

static inline uintptr_t
bfdev_radix_offset(uintptr_t index, size_t cells)
{
    size_t num;

    if (bfdev_pow2_check(cells))
        return index * cells;

    /* object per block */
    num = BFDEV_RADIX_BLOCK / cells;
    return (index / num) * BFDEV_RADIX_BLOCK +
           (index % num) * cells;
}

static inline uintptr_t
bfdev_radix_index(uintptr_t offset, size_t cells)
{
    size_t num;

    if (bfdev_pow2_check(cells))
        return offset / cells;

    /* object per block */
    num = BFDEV_RADIX_BLOCK / cells;
    return (offset / num) / BFDEV_RADIX_BLOCK +
           (offset % num) / cells;
}

#define bfdev_radix_cast(radix) \
    (typeof((radix)->data) *)

#define bfdev_radix_cells(radix) \
    sizeof((radix)->data)

#define bfdev_radix_to_offset(radix, index) \
    bfdev_radix_offset(index, bfdev_radix_cells(radix))

#define bfdev_radix_to_index(radix, offset) \
    bfdev_radix_index(offset, bfdev_radix_cells(radix))

#define bfdev_radix_find(radix, index) ({       \
    bfdev_radix_root_t *__root;                 \
    __root = &(radix)->tree;                    \
    bfdev_radix_cast(radix) (                   \
        bfdev_radix_root_find(__root,           \
            bfdev_radix_to_offset(radix, index) \
        )                                       \
    );                                          \
})

#define bfdev_radix_alloc(radix, index) ({      \
    bfdev_radix_root_t *__root;                 \
    __root = &(radix)->tree;                    \
    bfdev_radix_cast(radix) (                   \
        bfdev_radix_root_alloc(__root,          \
            bfdev_radix_to_offset(radix, index) \
        )                                       \
    );                                          \
})

#define bfdev_radix_free(radix, index) ({   \
    bfdev_radix_root_t *__root;             \
    __root = &(radix)->tree;                \
    bfdev_radix_root_free(__root,           \
        bfdev_radix_to_offset(radix, index) \
    );                                      \
})

#define bfdev_radix_charge(radix, index, size) ({   \
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_charge(__root,                 \
        bfdev_radix_to_offset(radix, index),        \
        bfdev_radix_to_offset(radix, size)          \
    );                                              \
})

#define bfdev_radix_release(radix) ({   \
    bfdev_radix_root_t *__root;         \
    __root = &(radix)->tree;            \
    bfdev_radix_root_release(__root);   \
})

#define bfdev_radix_first(radix, index) ({              \
    bfdev_radix_root_t *__root;                         \
    uintptr_t __off;                                    \
    void *__retval;                                     \
    __root = &(radix)->tree;                            \
    __retval = bfdev_radix_root_first(__root, &__off);  \
    *(index) = bfdev_radix_to_index(radix, __off);      \
    __retval;                                           \
})

#define bfdev_radix_last(radix, index) ({               \
    bfdev_radix_root_t *__root;                         \
    uintptr_t __off;                                    \
    void *__retval;                                     \
    __root = &(radix)->tree;                            \
    __retval = bfdev_radix_root_last(__root, &__off);   \
    *(index) = bfdev_radix_to_index(radix, __off);      \
    __retval;                                           \
})

#define bfdev_radix_next(radix, index) ({               \
    bfdev_radix_root_t *__root;                         \
    uintptr_t __off;                                    \
    void *__retval;                                     \
    __root = &(radix)->tree;                            \
    __off = bfdev_radix_to_offset(radix, *(index));     \
    __retval = bfdev_radix_root_next(__root, &__off);   \
    *(index) = bfdev_radix_to_index(radix, __off);      \
    __retval;                                           \
})

#define bfdev_radix_prev(radix, index) ({               \
    bfdev_radix_root_t *__root;                         \
    uintptr_t __off;                                    \
    void *__retval;                                     \
    __root = &(radix)->tree;                            \
    __off = bfdev_radix_to_offset(radix, *(index));     \
    __retval = bfdev_radix_root_prev(__root, &__off);   \
    *(index) = bfdev_radix_to_index(radix, __off);      \
    __retval;                                           \
})

#define bfdev_radix_for_each(value, radix, index) \
    for ((value) = bfdev_radix_first(radix, index); \
         value; (value) = bfdev_radix_next(radix, index))

#define bfdev_radix_for_each_reverse(value, radix, index) \
    for ((value) = bfdev_radix_last(radix, index); \
         value; (value) = bfdev_radix_prev(radix, index))

#define bfdev_radix_for_each_form(value, radix, index) \
    for (; value; (value) = bfdev_radix_next(radix, index))

#define bfdev_radix_for_each_reverse_form(value, radix, index) \
    for (; value; (value) = bfdev_radix_prev(radix, index))

#define bfdev_radix_for_each_continue(value, radix, index) \
    for ((value) = bfdev_radix_next(radix, index); \
         value; (value) = bfdev_radix_next(radix, index))

#define bfdev_radix_for_each_reverse_continue(value, radix, index) \
    for ((value) = bfdev_radix_prev(radix, index); \
         value; (value) = bfdev_radix_prev(radix, index))

#define bfdev_radix_for_each_safe(value, radix, index, tval, tidx) \
    for ((void)(((value) = bfdev_radix_first(radix, index)) && \
         (*(tidx) = *(index), (tval) = bfdev_radix_next(radix, tidx))); value; \
         (void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_next(radix, tidx))))

#define bfdev_radix_for_each_reverse_safe(value, radix, index, tval, tidx) \
    for ((void)(((value) = bfdev_radix_last(radix, index)) && \
         (*(tidx) = *(index), (tval) = bfdev_radix_prev(radix, tidx))); value; \
         (void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_prev(radix, tidx))))

#define bfdev_radix_for_each_form_safe(value, radix, index, tval, tidx) \
    for (; value; (void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_next(radix, tidx))))

#define bfdev_radix_for_each_reverse_form_safe(value, radix, index, tval, tidx) \
    for (; value; (void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_prev(radix, tidx))))

#define bfdev_radix_for_each_continue_safe(value, radix, index, tval, tidx) \
    for ((void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_next(radix, tidx))); value; \
         (void)((*(index) = *(tidx), (value) = (tval)) && \
         (tval) = bfdev_radix_next(radix, tidx)))

#define bfdev_radix_for_each_reverse_continue_safe(value, radix, index, tval, tidx) \
    for ((void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_prev(radix, tidx))); value; \
         (void)((*(index) = *(tidx), (value) = (tval)) && \
         ((tval) = bfdev_radix_prev(radix, tidx))))

extern void *
bfdev_radix_root_find(bfdev_radix_root_t *root, uintptr_t offset);

extern void *
bfdev_radix_root_alloc(bfdev_radix_root_t *root, uintptr_t offset);

extern int
bfdev_radix_root_free(bfdev_radix_root_t *root, uintptr_t offset);

extern int
bfdev_radix_root_charge(bfdev_radix_root_t *root, uintptr_t offset, size_t size);

extern void
bfdev_radix_root_release(bfdev_radix_root_t *root);

extern void *
bfdev_radix_root_first(bfdev_radix_root_t *root, uintptr_t *offsetp);

extern void *
bfdev_radix_root_last(bfdev_radix_root_t *root, uintptr_t *offsetp);

extern void *
bfdev_radix_root_next(bfdev_radix_root_t *root, uintptr_t *offsetp);

extern void *
bfdev_radix_root_prev(bfdev_radix_root_t *root, uintptr_t *offsetp);

BFDEV_END_DECLS

#endif /* _BFDEV_RADIX_H_ */
