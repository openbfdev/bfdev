/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "log-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/log.h>

int
main(int argc, const char *argv[])
{
    bfdev_log_emerg("helloworld\n");
    bfdev_log_alert("helloworld\n");
    bfdev_log_crit("helloworld\n");
    bfdev_log_err("helloworld\n");
    bfdev_log_warn("helloworld\n");
    bfdev_log_notice("helloworld\n");
    bfdev_log_info("helloworld\n");
    bfdev_log_debug("helloworld\n");
    bfdev_log("helloworld\n");

    return 0;
}
