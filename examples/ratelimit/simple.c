/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "ratelimit-simple"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <time.h>
#include <unistd.h>
#include <bfdev/log.h>
#include <bfdev/ratelimit.h>
#include <bfdev/bug.h>

static bfdev_time_t
current_msec(void)
{
    struct timespec ts;

    BFDEV_BUG_ON(clock_gettime(CLOCK_REALTIME, &ts));

    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int
main(int argc, char *argv[])
{
    BFDEV_DEFINE_RATELIMIT(limit, 100, 3);
    unsigned int count;
    bool accept;

    for (count = 0; count < 10; ++count) {
        accept = bfdev_ratelimit(&limit, current_msec());
        bfdev_log("ratelimit %u: %d\n", count, accept);
        usleep(20 * 1000); /* 20ms */
    }

    return 0;
}
