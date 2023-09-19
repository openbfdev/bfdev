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

#define BFDEV_RADIX_SHIFT 8
#define BFDEV_RADIX_BLOCK BFDEV_BIT(BFDEV_RADIX_SHIFT)

#define BFDEV_RADIX_ARY (BFDEV_RADIX_BLOCK / sizeof(struct bfdev_radix_node *))
#define BFDEV_RADIX_ARY_MASK (BFDEV_RADIX_ARY - 1)
#define BFDEV_RADIX_ARY_SHIFT bfdev_ilog2(BFDEV_RADIX_ARY)

struct bfdev_radix_root {
    const struct bfdev_alloc *alloc;
    struct bfdev_radix_node *node;
    unsigned int level;
};

struct bfdev_radix_node {
    struct bfdev_radix_node *parent;
    union {
        struct {
            struct bfdev_radix_node *child[BFDEV_RADIX_ARY];
            unsigned int refcount;
        };
        struct {
            uint8_t block[BFDEV_RADIX_BLOCK];
            BFDEV_DEFINE_BITMAP(bitmap, BFDEV_RADIX_BLOCK);
        };
    };
};

#define BFDEV_GENERIC_RADIX(datatype)   \
    union {                             \
        struct bfdev_radix_root tree;   \
        datatype data;                  \
    }

#define BFDEV_RADIX_STATIC(ALLOC) \
    {.tree = {.alloc = (ALLOC)}}

#define BFDEV_RADIX_INIT(name, alloc) \
    (typeof(name)) BFDEV_RADIX_STATIC(alloc)

#define BFDEV_RADIX_ROOT(name, type, alloc) \
    BFDEV_GENERIC_RADIX(type) name = BFDEV_RADIX_INIT(name, alloc)

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

#define bfdev_radix_cast(radix) \
    (typeof((radix)->data) *)

#define bfdev_radix_cells(radix) \
    sizeof((radix)->data)

#define bfdev_radix_to_offset(radix, index) \
    bfdev_radix_offset(index, bfdev_radix_cells(radix))

#define bfdev_radix_find(radix, index) ({           \
    struct bfdev_radix_root *__root;                \
    __root = &(radix)->tree;                        \
    bfdev_radix_cast(radix) (                       \
        bfdev_radix_root_find(__root,               \
            bfdev_radix_to_offset(radix, index)     \
        )                                           \
    );                                              \
})

#define bfdev_radix_alloc(radix, index) ({          \
    struct bfdev_radix_root *__root;                \
    __root = &(radix)->tree;                        \
    bfdev_radix_cast(radix) (                       \
        bfdev_radix_root_alloc(__root,              \
            bfdev_radix_to_offset(radix, index)     \
        )                                           \
    );                                              \
})

#define bfdev_radix_free(radix, index) ({           \
    struct bfdev_radix_root *__root;                \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_free(__root,                   \
        bfdev_radix_to_offset(radix, index)         \
    );                                              \
})

#define bfdev_radix_charge(radix, index, size) ({   \
    struct bfdev_radix_root *__root;                \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_charge(__root,                 \
        bfdev_radix_to_offset(radix, index),        \
        bfdev_radix_to_offset(radix, size)          \
    );                                              \
})

#define bfdev_radix_destory(radix) ({               \
    struct bfdev_radix_root *__root;                \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_destory(__root);               \
})

extern void *
bfdev_radix_root_find(struct bfdev_radix_root *root, uintptr_t offset);

extern void *
bfdev_radix_root_alloc(struct bfdev_radix_root *root, uintptr_t offset);

extern int
bfdev_radix_root_free(struct bfdev_radix_root *root, uintptr_t offset);

extern int
bfdev_radix_root_charge(struct bfdev_radix_root *root, uintptr_t offset, size_t size);

extern void
bfdev_radix_root_destory(struct bfdev_radix_root *root);

BFDEV_END_DECLS

#endif /* _BFDEV_RADIX_H_ */
