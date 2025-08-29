/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#define MODULE_NAME "log-modules"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/log.h>
#include <bfdev/modules/log.h>
#include <unistd.h>

int
main(int argc, const char *argv[])
{
    unsigned int count;

    bfdev_log_localtime_enable(&bfdev_log_default);
    bfdev_log_hostname_enable(&bfdev_log_default);
    bfdev_log_pid_enable(&bfdev_log_default);

    for (count = 0; count < 3; ++count) {
        bfdev_log_info("Hello World!\n");
        sleep(1);
    }

    return 0;
}
