/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include "log.h"
#include <bfdev/log.h>
#include <export.h>

static const char * const
level_name[] = {
    [BFDEV_LEVEL_EMERG  ] = "emerg",
    [BFDEV_LEVEL_ALERT  ] = "alert",
    [BFDEV_LEVEL_CRIT   ] = "crit",
    [BFDEV_LEVEL_ERR    ] = "error",
    [BFDEV_LEVEL_WARNING] = "warning",
    [BFDEV_LEVEL_NOTICE ] = "notice",
    [BFDEV_LEVEL_INFO   ] = "info",
    [BFDEV_LEVEL_DEBUG  ] = "debug",
    [BFDEV_LEVEL_DEFAULT] = "default",
};

int
log_level_prefix(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    if (!bfdev_log_level_test(log))
        return -BFDEV_ENOERR;

    return bfdev_msg_append(msg, "[%s] ", level_name[msg->level]);
}
