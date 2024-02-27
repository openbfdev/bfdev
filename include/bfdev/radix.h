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

#define BFDEV_RADIX_ARY (BFDEV_RADIX_BLOCK / sizeof(bfdev_radix_node_t *))
#define BFDEV_RADIX_ARY_MASK (BFDEV_RADIX_ARY - 1)
#define BFDEV_RADIX_ARY_SHIFT bfdev_ilog2(BFDEV_RADIX_ARY)

typedef struct bfdev_radix_root bfdev_radix_root_t;
typedef struct bfdev_radix_node bfdev_radix_node_t;

struct bfdev_radix_root {
    const bfdev_alloc_t *alloc;
    bfdev_radix_node_t *node;
    unsigned int level;
};

struct bfdev_radix_node {
    bfdev_radix_node_t *parent;
    union {
        struct {
            bfdev_radix_node_t *child[BFDEV_RADIX_ARY];
            unsigned int refcount;
        };
        struct {
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
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_cast(radix) (                       \
        bfdev_radix_root_find(__root,               \
            bfdev_radix_to_offset(radix, index)     \
        )                                           \
    );                                              \
})

#define bfdev_radix_alloc(radix, index) ({          \
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_cast(radix) (                       \
        bfdev_radix_root_alloc(__root,              \
            bfdev_radix_to_offset(radix, index)     \
        )                                           \
    );                                              \
})

#define bfdev_radix_free(radix, index) ({           \
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_free(__root,                   \
        bfdev_radix_to_offset(radix, index)         \
    );                                              \
})

#define bfdev_radix_charge(radix, index, size) ({   \
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_charge(__root,                 \
        bfdev_radix_to_offset(radix, index),        \
        bfdev_radix_to_offset(radix, size)          \
    );                                              \
})

#define bfdev_radix_destory(radix) ({               \
    bfdev_radix_root_t *__root;                     \
    __root = &(radix)->tree;                        \
    bfdev_radix_root_destory(__root);               \
})

extern void *
bfdev_radix_root_find(bfdev_radix_root_t *root, uintptr_t offset);

extern void *
bfdev_radix_root_alloc(bfdev_radix_root_t *root, uintptr_t offset);

extern int
bfdev_radix_root_free(bfdev_radix_root_t *root, uintptr_t offset);

extern int
bfdev_radix_root_charge(bfdev_radix_root_t *root, uintptr_t offset, size_t size);

extern void
bfdev_radix_root_destory(bfdev_radix_root_t *root);

BFDEV_END_DECLS

#endif /* _BFDEV_RADIX_H_ */
