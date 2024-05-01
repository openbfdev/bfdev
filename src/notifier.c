/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/notifier.h>
#include <export.h>

#define ilist_to_notifier(ptr) \
    bfdev_ilist_entry(ptr, bfdev_notifier_node_t, list)

static long
notifier_chain_cmp(const bfdev_ilist_node_t *node1,
                   const bfdev_ilist_node_t *node2, void *pdata)
{
    int prio1, prio2;

    prio1 = ilist_to_notifier(node1)->priority;
    prio2 = ilist_to_notifier(node2)->priority;

    return bfdev_cmp(prio1 > prio2);
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

    return -BFDEV_ENOERR;
}

export void
bfdev_notifier_unregister(bfdev_notifier_t *head, bfdev_notifier_node_t *node)
{
    bfdev_ilist_del(&head->nodes, &node->list);
}
