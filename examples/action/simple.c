/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "action-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev/action.h>
#include <bfdev/log.h>

static int
test_action(void *pdata)
{
    bfdev_log_info("%s\n", (char *)pdata);
    return 0;
}

int main(int argc, char **argv)
{
    BFDEV_DEFINE_ACTION(action, test_action, NULL);
    int retval;

    bfdev_action_update(&action, "hello world");
    retval = bfdev_action_call(&action);
    if (retval)
        return retval;

    bfdev_action_update(&action, "wow bfdev");
    retval = bfdev_action_call(&action);
    if (retval)
        return retval;

    return 0;
}
