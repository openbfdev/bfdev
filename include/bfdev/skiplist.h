/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_SKIPLIST_H_
#define _BFDEV_SKIPLIST_H_

#include <errno.h>
#include "list.h"

typedef long (*skiplist_find_t)(const void *data, const void *key);
typedef long (*skiplist_cmp_t)(const void *dataa, const void *datab);
typedef void (*skiplist_release_t)(void *data);

struct skip_node {
    void *pdata;
    struct list_head list[0];
};

struct skip_head {
    unsigned int curr;
    unsigned int levels;
    struct list_head nodes[0];
};

extern int skiplist_insert(struct skip_head *head, void *data, skiplist_cmp_t cmp);
extern void skiplist_delete(struct skip_head *head, void *key, skiplist_find_t find);
extern void *skiplist_find(struct skip_head *head, void *key, skiplist_find_t find);

extern void skiplist_reset(struct skip_head *head, skiplist_release_t relse);
extern void skiplist_destroy(struct skip_head *head, skiplist_release_t relse);
extern struct skip_head *skiplist_create(unsigned int levels);

#endif /* _BFDEV_SKIPLIST_H_ */
