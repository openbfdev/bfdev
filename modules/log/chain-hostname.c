/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/modules/log.h>
#include <sys/utsname.h>
#include <export.h>

export int
bfdev_log_chain_hostname(bfdev_log_message_t *msg, void *pdata)
{
    struct utsname sys_info;

    if (uname(&sys_info) < 0)
        return -BFDEV_EFAULT;

    return bfdev_msg_append(msg, "%s ", sys_info.nodename);
}
