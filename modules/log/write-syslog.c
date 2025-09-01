/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/modules/log.h>
#include <syslog.h>
#include <export.h>

static const int
syslog_level[] = {
    [BFDEV_LEVEL_EMERG  ] = LOG_EMERG,
    [BFDEV_LEVEL_ALERT  ] = LOG_ALERT,
    [BFDEV_LEVEL_CRIT   ] = LOG_CRIT,
    [BFDEV_LEVEL_ERR    ] = LOG_ERR,
    [BFDEV_LEVEL_WARNING] = LOG_WARNING,
    [BFDEV_LEVEL_NOTICE ] = LOG_NOTICE,
    [BFDEV_LEVEL_INFO   ] = LOG_INFO,
    [BFDEV_LEVEL_DEBUG  ] = LOG_DEBUG,
};

export int
bfdev_log_write_syslog(bfdev_log_message_t *msg, void *pdata)
{
    int level, priority;

    level = msg->level;
    if (level >= BFDEV_ARRAY_SIZE(syslog_level))
        level = BFDEV_LEVEL_INFO;

    priority = syslog_level[level];
    syslog(priority, "%.*s", (int)msg->length, msg->buff);

    return msg->length;
}
