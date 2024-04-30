/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/scnprintf.h>
#include <bfdev/bug.h>
#include <export.h>

export
BFDEV_DEFINE_LOG(
    bfdev_log_default,
    BFDEV_LEVEL_DEFAULT, BFDEV_LEVEL_DEFAULT,
    BFDEV_LOG_COLOR | BFDEV_LOG_LEVEL,
    NULL, NULL
);

#define __INSIDE_LOG__
#include <port/log.h>

#include "color.c"
#include "level.c"

static inline char
log_get_level(const char *str)
{
    if (str[0] == BFDEV_SOH_ASCII && str[1])
        return str[1];

    return 0;
}

extern unsigned int
bfdev_log_level(const char *str, const char **endptr)
{
    unsigned int level;
    char value;

    for (level = BFDEV_LEVEL_DEFAULT; *str; str += 2) {
        value = log_get_level(str);
        if (!value)
            break;

        switch (value) {
            case '0' ... '9':
                level = value - '0';
                break;

            default:
                break;
        }
    }

    if (*endptr)
        *endptr = str;

    return level;
}

static int
log_prefix(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    log_level(log, msg);
    log_color_prefix(log, msg);

    return -BFDEV_ENOERR;
}

static int
log_suffix(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    log_color_suffix(log, msg);

    return -BFDEV_ENOERR;
}

static int
log_emit(bfdev_log_t *log, unsigned int level, const char *fmt, va_list args)
{
    char buff[BFDEV_LOG_BUFF_SIZE];
    bfdev_log_message_t msg;
    int retval;

    if (level >= BFDEV_LEVEL_DEFAULT)
        level = log->default_level;

    if (level > log->record_level)
        return 0;

    msg.level = level;
    msg.buff = buff;
    msg.length = 0;

    retval = log_prefix(log, &msg);
    if (bfdev_unlikely(retval))
        return retval;

    msg.length += bfdev_vscnprintf(
        buff + msg.length, BFDEV_LOG_BUFF_SIZE - msg.length,
        fmt, args
    );

    retval = log_suffix(log, &msg);
    if (bfdev_unlikely(retval))
        return retval;

    if (log->write)
        retval = log->write(&msg, log->pdata);
    else
        retval = generic_log_write(&msg);

    return retval;
}

export int
bfdev_log_state_vprint(bfdev_log_t *log, const char *fmt, va_list args)
{
    unsigned int level;

    level = bfdev_log_level(fmt, &fmt);

    return log_emit(log, level, fmt, args);
}

export int
bfdev_log_state_print(bfdev_log_t *log, const char *fmt, ...)
{
    va_list para;
    int length;

    va_start(para, fmt);
    length = bfdev_log_state_vprint(log, fmt, para);
    va_end(para);

    return length;
}