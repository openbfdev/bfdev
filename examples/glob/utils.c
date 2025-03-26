/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bfdev-glob"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdbool.h>
#include <bfdev/log.h>
#include <bfdev/glob.h>

int
main(int argc, const char *argv[])
{
    unsigned int count;
    const char *patten;
    bool result;

    if (argc < 2) {
        bfdev_log_alert("Usage: %s patten string ...\n", argv[0]);
        return 1;
    }

    patten = argv[1];
    for (count = 2; count < argc; ++count) {
        result = bfdev_glob(patten, argv[count]);
        bfdev_log_info("matching %s: %s\n", argv[count], result ? "yes" : "no");
    }

    return 0;
}
