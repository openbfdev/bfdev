/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "notifier-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/log.h>
#include <bfdev/notifier.h>

static
BFDEV_DEFINE_NOTIFIER(notifer);

static bfdev_notifier_ret_t
func(void *arg, void *pdata)
{
    bfdev_log_info("%s: %s\n", (char *)arg, (char *)pdata);
    return BFDEV_NOTIFI_RET_DONE;
}

int main(int argc, const char *argv[])
{
    bfdev_notifier_node_t node[3];
    int retval;

    node[0].priority = 3;
    node[0].entry = func;
    node[0].pdata = "priority=3";

    retval = bfdev_notifier_register(&notifer, &node[0]);
    if (retval)
        return retval;

    node[1].priority = 2;
    node[1].entry = func;
    node[1].pdata = "priority=2";

    retval = bfdev_notifier_register(&notifer, &node[1]);
    if (retval)
        return retval;

    node[2].priority = 1;
    node[2].entry = func;
    node[2].pdata = "priority=1";

    retval = bfdev_notifier_register(&notifer, &node[2]);
    if (retval)
        return retval;

    return bfdev_notifier_call(&notifer, "helloworld", -1, NULL);
}
