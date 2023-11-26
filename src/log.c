/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/log.h>
#include <bfdev/scnprintf.h>
#include <export.h>

static const unsigned int
bfdev_level_color[] = {
    [BFDEV_LEVEL_EMERG   ] = BFDEV_COLR_RED,
    [BFDEV_LEVEL_ALERT   ] = BFDEV_COLR_DARK_MAGENTA,
    [BFDEV_LEVEL_CRIT    ] = BFDEV_COLR_MAGENTA,
    [BFDEV_LEVEL_ERR     ] = BFDEV_COLR_YELLOW,
    [BFDEV_LEVEL_WARNING ] = BFDEV_COLR_BLUE,
    [BFDEV_LEVEL_NOTICE  ] = BFDEV_COLR_CYAN,
    [BFDEV_LEVEL_INFO    ] = BFDEV_COLR_GREEN,
    [BFDEV_LEVEL_DEBUG   ] = BFDEV_COLR_DARK_GRAY,
    [BFDEV_LEVEL_DEFAULT ] = BFDEV_COLR_DEFAULT,
};

static const char * const
bfdev_level_name[] = {
    [BFDEV_LEVEL_EMERG   ] = "emerg",
    [BFDEV_LEVEL_ALERT   ] = "alert",
    [BFDEV_LEVEL_CRIT    ] = "crit",
    [BFDEV_LEVEL_ERR     ] = "error",
    [BFDEV_LEVEL_WARNING ] = "warning",
    [BFDEV_LEVEL_NOTICE  ] = "notice",
    [BFDEV_LEVEL_INFO    ] = "info",
    [BFDEV_LEVEL_DEBUG   ] = "debug",
    [BFDEV_LEVEL_DEFAULT ] = "default",
};

export struct bfdev_log
bfdev_log_default = {
    .default_level = BFDEV_LEVEL_DEFAULT,
    .record_level = BFDEV_LEVEL_DEFAULT,
    .flags = BFDEV_LOG_COLOR | BFDEV_LOG_COMMIT,
};

static void
generic_write(const char *buff)
{
    printf("%s", buff);
}

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
    char value, klevel;

    for (klevel = BFDEV_LEVEL_DEFAULT; *str; str += 2) {
        value = log_get_level(str);
        if (!value)
            break;

        switch (value) {
            case '0' ... '9':
                klevel = value - '0';
                break;

            default:
                break;
        }
    }

    if (*endptr)
        *endptr = str;

    return klevel;
}

export int
bfdev_log_state_vprint(struct bfdev_log *log, const char *fmt, va_list args)
{
    char buff[BFDEV_LOG_BUFF_SIZE];
    int retval, offset = 0;
    unsigned int level;

    level = bfdev_log_level(fmt, &fmt);
    if (level >= BFDEV_LEVEL_DEFAULT)
        level = log->default_level;

    if (bfdev_unlikely(level > log->record_level))
        return 0;

    if (bfdev_log_test_commit(log)) {
        retval = bfdev_scnprintf(
            buff + offset, BFDEV_LOG_BUFF_SIZE - offset,
            "[%s] ", bfdev_level_name[level]
        );
        offset += retval;
    }

    if (bfdev_log_test_color(log)) {
        retval = bfdev_scnprintf(
            buff + offset, BFDEV_LOG_BUFF_SIZE - offset,
            "\e[%dm", bfdev_level_color[level]
        );
        offset += retval;
    }

    retval = bfdev_vscnprintf(
        buff + offset, BFDEV_LOG_BUFF_SIZE - offset,
        fmt, args
    );
    offset += retval;

    if (bfdev_log_test_color(log)) {
        retval = bfdev_scnprintf(
            buff + offset, BFDEV_LOG_BUFF_SIZE - offset,
            "\e[0m"
        );
        offset += retval;
    }

    if (log->write)
        return log->write(buff, offset, log->pdata);
    generic_write(buff);

    return offset;
}

export int
bfdev_log_state_print(struct bfdev_log *log, const char *fmt, ...)
{
    va_list para;
    int length;

    va_start(para,fmt);
    length = bfdev_log_state_vprint(log, fmt, para);
    va_end(para);

    return length;
}
