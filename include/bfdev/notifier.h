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

typedef struct bfdev_notifier bfdev_notifier_t;
typedef struct bfdev_notifier_node bfdev_notifier_node_t;
typedef enum bfdev_notifier_ret bfdev_notifier_ret_t;

typedef bfdev_notifier_ret_t
(*bfdev_notifier_entry_t)(void *args, void *pdata);

enum bfdev_notifier_ret {
    __BFDEV_NOTIFI_RET_REMOVE = 0,
    __BFDEV_NOTIFI_RET_STOP,

    BFDEV_NOTIFI_RET_DONE   = 0,
    BFDEV_NOTIFI_RET_REMOVE = BFDEV_BIT(__BFDEV_NOTIFI_RET_REMOVE),
    BFDEV_NOTIFI_RET_STOP   = BFDEV_BIT(__BFDEV_NOTIFI_RET_STOP),
};

/**
 * struct bfdev_notifier - generic notification chain header.
 * @nodes: node header for managing notification chain nodes.
 */
struct bfdev_notifier {
    bfdev_ilist_head_t nodes;
};

/**
 * struct bfdev_notifier_node - describe a notification chain node.
 * @list: node for hanging notification chain.
 * @priority: priority of notification chain node.
 * @entry: callback function of notification chain.
 * @pdata: private data of notification chain node.
 */
struct bfdev_notifier_node {
    bfdev_ilist_node_t list;
    int priority;

    bfdev_notifier_entry_t entry;
    void *pdata;
};

#define BFDEV_NOTIFIER_STATIC(HEAD) { \
    .nodes = BFDEV_ILIST_HEAD_STATIC(&(HEAD)->nodes), \
}

#define BFDEV_NOTIFIER_INIT(head) \
    (bfdev_notifier_t) BFDEV_NOTIFIER_STATIC(head)

#define BFDEV_DEFINE_NOTIFIER(name) \
    bfdev_notifier_t name = BFDEV_NOTIFIER_INIT(&name)

static inline void
bfdev_notifier_init(bfdev_notifier_t *head)
{
    *head = BFDEV_NOTIFIER_INIT(head);
}

/**
 * bfdev_notifier_call - call the callback function of node on notification chain.
 * @head: chain header to be notified.
 * @args: parameters to be passed to the node.
 * @call_num: number of nodes to be notified.
 * @called_num: number of nodes actually notified.
 */
extern bfdev_notifier_ret_t
bfdev_notifier_call(bfdev_notifier_t *head, void *args,
                    unsigned int call_num, unsigned int *called_num);

/**
 * bfdev_notifier_register - register a node to the notification chain.
 * @head: header to be registered.
 * @node: notification chain node to register.
 */
extern int
bfdev_notifier_register(bfdev_notifier_t *head, bfdev_notifier_node_t *node);

/**
 * bfdev_notifier_unregister - unregister a node from the notification chain.
 * @head: header to be unregistered.
 * @node: notification chain node to unregister.
 */
extern void
bfdev_notifier_unregister(bfdev_notifier_t *head, bfdev_notifier_node_t *node);

BFDEV_BEGIN_DECLS

#endif /* _BFDEV_NOTIFIER_H_ */
