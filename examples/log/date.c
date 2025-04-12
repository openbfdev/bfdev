/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "log-date"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <bfdev/log.h>
#include <bfdev/scnprintf.h>
#include <time.h>
#include <unistd.h>

static int
log_hook_date(bfdev_log_message_t *msg, void *pdata)
{
    struct tm *tm_info;
    time_t timestamp;
    char buff[64];

    time(&timestamp);
    tm_info = localtime(&timestamp);
    strftime(buff, sizeof(buff), "%a %b %d %H:%M:%S %p %Z %Y", tm_info);

    return bfdev_msg_append(msg, "<%s> ", buff);
}

static bfdev_log_chain_t
log_chain_date = {
    .func = log_hook_date,
    .priority = 0,
};

int
main(int argc, const char *argv[])
{
    unsigned int count;

    bfdev_log_hook_register(&bfdev_log_default, &log_chain_date);
    for (count = 0; count < 3; ++count) {
        bfdev_log_info("Hello World!\n");
        sleep(1);
    }

    return 0;
}
