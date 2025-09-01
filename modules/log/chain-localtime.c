/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/modules/log.h>
#include <time.h>
#include <export.h>

export int
bfdev_log_chain_localtime(bfdev_log_message_t *msg, void *pdata)
{
    struct tm *tm_info;
    time_t timestamp;
    char buff[64];

    time(&timestamp);
    tm_info = localtime(&timestamp);
    strftime(buff, sizeof(buff), "%a %b %d %H:%M:%S %p %Z %Y", tm_info);

    return bfdev_msg_append(msg, "<%s> ", buff);
}
