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
    BFDEV_DEFINE_LOG_CHAIN(log_localtime, bfdev_log_chain_localtime, -300, NULL);
    BFDEV_DEFINE_LOG_CHAIN(log_hostname, bfdev_log_chain_hostname, -200, NULL);
    BFDEV_DEFINE_LOG_CHAIN(log_pid, bfdev_log_chain_pid, -100, NULL);
    unsigned int count;

    bfdev_log_chain_register(&bfdev_log_default, &log_localtime);
    bfdev_log_chain_register(&bfdev_log_default, &log_hostname);
    bfdev_log_chain_register(&bfdev_log_default, &log_pid);

    for (count = 0; count < 3; ++count) {
        bfdev_log_info("Hello World!\n");
        sleep(1);
    }

    return 0;
}
