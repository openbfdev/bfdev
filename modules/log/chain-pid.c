/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/modules/log.h>
#include <unistd.h>
#include <export.h>

export int
bfdev_log_chain_pid(bfdev_log_message_t *msg, void *pdata)
{
    return bfdev_msg_append(msg, "(pid: %d) ", getpid());
}
