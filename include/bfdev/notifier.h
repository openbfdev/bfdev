/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_NOTIFIER_H_
#define _BFDEV_NOTIFIER_H_

#include <bfdev/config.h>
#include <bfdev/ilist.h>
#include <bfdev/bits.h>

BFDEV_BEGIN_DECLS

struct bfdev_notifier_node;
typedef enum bfdev_notifier_ret bfdev_notifier_ret_t;

typedef bfdev_notifier_ret_t (*bfdev_notifier_entry_t)
(struct bfdev_notifier_node *node, void *args);

enum bfdev_notifier_ret {
    __BFDEV_NOTIFI_RET_DONE = 0,
    __BFDEV_NOTIFI_RET_WARN,
    __BFDEV_NOTIFI_RET_REMOVE,
    __BFDEV_NOTIFI_RET_STOP,

    BFDEV_NOTIFI_RET_DONE   = BFDEV_BIT(__BFDEV_NOTIFI_RET_DONE),
    BFDEV_NOTIFI_RET_WARN   = BFDEV_BIT(__BFDEV_NOTIFI_RET_WARN),
    BFDEV_NOTIFI_RET_REMOVE = BFDEV_BIT(__BFDEV_NOTIFI_RET_REMOVE),
    BFDEV_NOTIFI_RET_STOP   = BFDEV_BIT(__BFDEV_NOTIFI_RET_STOP),
};

/**
 * struct bfdev_notifier_node - describe a notification chain node.
 * @list: node for hanging notification chain.
 * @entry: callback function of notification chain.
 * @priority: priority of notification chain node.
 * @pdata: private data of notification chain node.
 */
struct bfdev_notifier_node {
    struct bfdev_ilist_node list;
    bfdev_notifier_entry_t entry;
    int priority;
    void *pdata;
};

/**
 * struct bfdev_notifier_head - generic notification chain header.
 * @node: node header for managing notification chain nodes.
 * @name: name of notification chain.
 */
struct bfdev_notifier_head {
    struct bfdev_ilist_head node;
    const char *name;
};

#define bfdev_ilist_to_notifier(ptr) \
    bfdev_ilist_entry(ptr, struct bfdev_notifier_node, list)

/**
 * bfdev_notifier_call - call the callback function of node on notification chain.
 * @head: chain header to be notified.
 * @arg: parameters to be passed to the node.
 * @call_num: number of nodes to be notified.
 * @called_num: number of nodes actually notified.
 */
extern bfdev_notifier_ret_t
bfdev_notifier_call(struct bfdev_notifier_head *head, void *args,
                    unsigned int call_num, unsigned int *called_num);

/**
 * bfdev_notifier_register - register a node to the notification chain.
 * @head: header to be registered.
 * @node: notification chain node to register.
 */
extern int
bfdev_notifier_register(struct bfdev_notifier_head *head, struct bfdev_notifier_node *node);

/**
 * bfdev_notifier_unregister - unregister a node from the notification chain.
 * @head: header to be unregistered.
 * @node: notification chain node to unregister.
 */
extern void
bfdev_notifier_unregister(struct bfdev_notifier_head *head, struct bfdev_notifier_node *node);

BFDEV_BEGIN_DECLS

#endif /* _BFDEV_NOTIFIER_H_ */
