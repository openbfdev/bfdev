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
    __BFDEV_JSON_IS_ARRAY = 0,
    __BFDEV_JSON_IS_OBJECT,
    __BFDEV_JSON_IS_STRING,
    __BFDEV_JSON_IS_NUMBER,
    __BFDEV_JSON_IS_NULL,
    __BFDEV_JSON_IS_TRUE,
    __BFDEV_JSON_IS_FALSE,

    BFDEV_JSON_IS_ARRAY   = BFDEV_BIT(__BFDEV_JSON_IS_ARRAY),
    BFDEV_JSON_IS_OBJECT  = BFDEV_BIT(__BFDEV_JSON_IS_OBJECT),
    BFDEV_JSON_IS_STRING  = BFDEV_BIT(__BFDEV_JSON_IS_STRING),
    BFDEV_JSON_IS_NUMBER  = BFDEV_BIT(__BFDEV_JSON_IS_NUMBER),
    BFDEV_JSON_IS_NULL    = BFDEV_BIT(__BFDEV_JSON_IS_NULL),
    BFDEV_JSON_IS_TRUE    = BFDEV_BIT(__BFDEV_JSON_IS_TRUE),
    BFDEV_JSON_IS_FALSE   = BFDEV_BIT(__BFDEV_JSON_IS_FALSE),
};

/**
 * struct bfdev_json_node - Describe a json structure.
 * @parent: point to the parent structure.
 * @sibling: list nodes hung on @child.
 * @flags: attributes bitmap.
 * @name: the name of the node.
 */
struct bfdev_json_node {
    struct bfdev_json_node *parent;
    struct bfdev_list_head sibling;
    unsigned long flags;
    const char *name;

    union {
        struct bfdev_list_head child;
        long number;
        char *string;
    };
};

BFDEV_BITFLAGS_STRUCT(bfdev_json, struct bfdev_json_node, flags)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           array, __BFDEV_JSON_IS_ARRAY)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           object, __BFDEV_JSON_IS_OBJECT)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           string, __BFDEV_JSON_IS_STRING)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           number, __BFDEV_JSON_IS_NUMBER)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           null, __BFDEV_JSON_IS_NULL)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           true, __BFDEV_JSON_IS_TRUE)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_json, struct bfdev_json_node, flags,
                           false, __BFDEV_JSON_IS_FALSE)

/**
 * bfdev_json_decode() - Decode json to syntax tree.
 * @alloc: allocator callback.
 * @buff: the json data to be decode.
 * @root: syntax tree root node pointer.
 */
extern int
bfdev_json_decode(const struct bfdev_alloc *alloc, const char *buff,
                  struct bfdev_json_node **root);

/**
 * bfdev_json_encode() - Encode syntax tree to json.
 * @root: syntax tree root node pointer.
 * @buff: buffer for outputting json.
 * @size: the size of @buff.
 */
extern int
bfdev_json_encode(struct bfdev_json_node *root, char *buff, int size);

/**
 * bfdev_json_release() - Release json syntax tree.
 * @alloc: allocator callback.
 * @root: syntax tree root node pointer.
 */
extern void
bfdev_json_release(const struct bfdev_alloc *alloc,
                   struct bfdev_json_node *root);

BFDEV_END_DECLS

#endif /* _BFDEV_JSON_H_ */
