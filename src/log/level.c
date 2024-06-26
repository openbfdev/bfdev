/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

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

static void
log_level(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    if (!bfdev_log_test_level(log))
        return;

    log_scnprintf(msg, "[%s] ", level_name[msg->level]);
}
