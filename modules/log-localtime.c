/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/log.h>
#include <bfdev/modules/log.h>
#include <time.h>
#include <export.h>

static int
log_hook_localtime(bfdev_log_message_t *msg, void *pdata)
{
    struct tm *tm_info;
    time_t timestamp;
    char buff[64];

    time(&timestamp);
    tm_info = localtime(&timestamp);
    strftime(buff, sizeof(buff), "%a %b %d %H:%M:%S %p %Z %Y", tm_info);

    return bfdev_msg_append(msg, "<%s> ", buff);
}

export bfdev_log_chain_t
bfdev_log_chain_localtime = {
    .func = log_hook_localtime,
    .priority = -1000,
};

export int
bfdev_log_localtime_enable(bfdev_log_t *log)
{
    return bfdev_log_hook_register(log, &bfdev_log_chain_localtime);
}

export void
bfdev_log_localtime_disable(bfdev_log_t *log)
{
    bfdev_log_hook_unregister(log, &bfdev_log_chain_localtime);
}
