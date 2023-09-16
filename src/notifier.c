/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-notifier"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev.h>
#include <bfdev/notifier.h>
#include <bfdev/log.h>
#include <export.h>

static long
notifier_chain_cmp(struct bfdev_ilist_node *nodea, struct bfdev_ilist_node *nodeb, const void *pdata)
{
    struct bfdev_notifier_node *nnodea = bfdev_ilist_to_notifier(nodea);
    struct bfdev_notifier_node *nnodeb = bfdev_ilist_to_notifier(nodeb);
    if (nnodea->priority == nnodeb->priority) return 0;
    return nnodea->priority < nnodeb->priority ? -1 : 1;
}

export bfdev_notifier_ret_t
bfdev_notifier_call(struct bfdev_notifier_head *head, void *arg,
                    unsigned int call_num, unsigned int *called_num)
{
    struct bfdev_notifier_node *node, *tmp;
    bfdev_notifier_ret_t retval = BFDEV_NOTIFI_RET_DONE;

    bfdev_ilist_for_each_entry_safe(node, tmp, &head->node, list) {
        if (!call_num--)
            break;

        bfdev_log_debug("chain '%s' calling (%p)\n", head->name, node);
        retval = node->entry(node, arg);

        if (called_num)
            (*called_num)++;

        if (retval & BFDEV_NOTIFI_RET_REMOVE) {
            bfdev_ilist_del(&head->node, &node->list);
            retval &= ~BFDEV_NOTIFI_RET_REMOVE;
        }

        if (retval & BFDEV_NOTIFI_RET_STOP)
            break;
    }

    return retval;
}

export int
bfdev_notifier_register(struct bfdev_notifier_head *head, struct bfdev_notifier_node *node)
{
    if (!node->entry)
        return -BFDEV_EINVAL;

    bfdev_ilist_node_init(&node->list);
    bfdev_ilist_add(&head->node, &node->list, notifier_chain_cmp, NULL);
    bfdev_log_debug("chain '%s' register (%p)\n", head->name, node);

    return -BFDEV_ENOERR;
}

export void
bfdev_notifier_unregister(struct bfdev_notifier_head *head, struct bfdev_notifier_node *node)
{
    bfdev_ilist_del(&head->node, &node->list);
    bfdev_log_debug("chain '%s' unregister (%p)\n", head->name, node);
}
