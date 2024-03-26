/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-notifier"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <base.h>
#include <bfdev/notifier.h>
#include <bfdev/log.h>
#include <export.h>

#define ilist_to_notifier(ptr) \
    bfdev_ilist_entry(ptr, bfdev_notifier_node_t, list)

static long
notifier_chain_cmp(const bfdev_ilist_node_t *node1,
                   const bfdev_ilist_node_t *node2, void *pdata)
{
    bfdev_notifier_node_t *nnode1, *nnode2;

    nnode1 = ilist_to_notifier(node1);
    nnode2 = ilist_to_notifier(node2);

    if (nnode1->priority == nnode2->priority)
        return 0;

    return nnode1->priority < nnode2->priority ? -1 : 1;
}

export bfdev_notifier_ret_t
bfdev_notifier_call(bfdev_notifier_t *head, void *arg,
                    unsigned int call_num, unsigned int *called_num)
{
    bfdev_notifier_node_t *node, *tmp;
    bfdev_notifier_ret_t retval;

    retval = BFDEV_NOTIFI_RET_DONE;
    bfdev_ilist_for_each_entry_safe(node, tmp, &head->nodes, list) {
        if (!call_num--)
            break;

        bfdev_log_debug("chain '%s' calling (%p)\n", head->name, node);
        retval = node->entry(arg, node->pdata);

        if (called_num)
            (*called_num)++;

        if (retval & BFDEV_NOTIFI_RET_REMOVE) {
            bfdev_ilist_del(&head->nodes, &node->list);
            retval &= ~BFDEV_NOTIFI_RET_REMOVE;
        }

        if (retval & BFDEV_NOTIFI_RET_STOP)
            break;
    }

    return retval;
}

export int
bfdev_notifier_register(bfdev_notifier_t *head, bfdev_notifier_node_t *node)
{
    if (bfdev_unlikely(!node->entry))
        return -BFDEV_EINVAL;

    bfdev_ilist_node_init(&node->list);
    bfdev_ilist_add(&head->nodes, &node->list, notifier_chain_cmp, NULL);
    bfdev_log_debug("chain '%s' register (%p)\n", head->name, node);

    return -BFDEV_ENOERR;
}

export void
bfdev_notifier_unregister(bfdev_notifier_t *head, bfdev_notifier_node_t *node)
{
    bfdev_ilist_del(&head->nodes, &node->list);
    bfdev_log_debug("chain '%s' unregister (%p)\n", head->name, node);
}
