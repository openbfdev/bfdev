/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "log-syslog"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/modules/log.h>
#include <syslog.h>

int
main(int argc, const char *argv[])
{
    openlog(MODULE_NAME, LOG_PID | LOG_CONS, LOG_DAEMON);
    bfdev_log_default.write = bfdev_log_write_syslog;
    bfdev_log("helloworld\n");

    return 0;
}
