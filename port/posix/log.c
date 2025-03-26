/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <port/stdio.h>
#include <port/log.h>
#include <stdio.h>
#include <export.h>

export int
bfport_log_write(bfdev_log_message_t *msg)
{
    FILE *file;

    if (msg->level > BFDEV_LEVEL_WARNING)
        file = stdout;
    else {
        file = stderr;
        fflush(stdout);
    }

    return fwrite(msg->buff, msg->length, 1, file);
}
