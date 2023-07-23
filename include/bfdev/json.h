/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_JSON_H_
#define _BFDEV_JSON_H_

#include <bfdev/config.h>
#include <bfdev/allocator.h>
#include <bfdev/bitflags.h>
#include <bfdev/list.h>

BFDEV_BEGIN_DECLS

enum bfdev_json_flags {
    __JSON_IS_ARRAY = 0,
    __JSON_IS_OBJECT,
    __JSON_IS_STRING,
    __JSON_IS_NUMBER,
    __JSON_IS_NULL,
    __JSON_IS_TRUE,
    __JSON_IS_FALSE,

    JSON_IS_ARRAY   = BFDEV_BIT(__JSON_IS_ARRAY),
    JSON_IS_OBJECT  = BFDEV_BIT(__JSON_IS_OBJECT),
    JSON_IS_STRING  = BFDEV_BIT(__JSON_IS_STRING),
    JSON_IS_NUMBER  = BFDEV_BIT(__JSON_IS_NUMBER),
    JSON_IS_NULL    = BFDEV_BIT(__JSON_IS_NULL),
    JSON_IS_TRUE    = BFDEV_BIT(__JSON_IS_TRUE),
    JSON_IS_FALSE   = BFDEV_BIT(__JSON_IS_FALSE),
};

struct bfdev_json_node {
    struct bfdev_json_node *parent;
    struct bfdev_list_head sibling;
    const char *name;
    unsigned long flags;
    union {
        struct bfdev_list_head child;
        long number;
        char *string;
    };
};

BFDEV_BITFLAGS_STRUCT(bfdev_json, struct bfdev_json_node, flags)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, array, __JSON_IS_ARRAY)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, object, __JSON_IS_OBJECT)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, string, __JSON_IS_STRING)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, number, __JSON_IS_NUMBER)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, null, __JSON_IS_NULL)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, true, __JSON_IS_TRUE)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags, false, __JSON_IS_FALSE)

extern int
bfdev_json_decode(const struct bfdev_alloc *alloc, const char *buff,
                  struct bfdev_json_node **root);

extern int
bfdev_json_encode(struct bfdev_json_node *root, char *buff, int size);

extern void
bfdev_json_release(const struct bfdev_alloc *alloc, struct bfdev_json_node *root);

BFDEV_END_DECLS

#endif /* _BFDEV_JSON_H_ */
