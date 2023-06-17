/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/log.h>
#include <export.h>

static const unsigned int
bfdev_level_color[] = {
    [BFDEV_LEVEL_EMERG]     = BFDEV_COLR_RED,
    [BFDEV_LEVEL_ALERT]     = BFDEV_COLR_DARK_MAGENTA,
    [BFDEV_LEVEL_CRIT]      = BFDEV_COLR_MAGENTA,
    [BFDEV_LEVEL_ERR]       = BFDEV_COLR_YELLOW,
    [BFDEV_LEVEL_WARNING]   = BFDEV_COLR_BLUE,
    [BFDEV_LEVEL_NOTICE]    = BFDEV_COLR_CYAN,
    [BFDEV_LEVEL_INFO]      = BFDEV_COLR_GREEN,
    [BFDEV_LEVEL_DEBUG]     = BFDEV_COLR_DARK_GRAY,
    [BFDEV_LEVEL_DEFAULT]   = BFDEV_COLR_DEFAULT,
};

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
bfdev_log_vprint(const char *fmt, va_list args)
{
    unsigned int level;
    int length;

    level = bfdev_log_level(fmt, &fmt);

    printf("\e[%dm", bfdev_level_color[level]);
    length = vprintf(fmt, args);
    printf("\e[0m");

    return length;
}

export int
bfdev_log_print(const char *fmt, ...)
{
    va_list para;
    int length;

    va_start(para,fmt);
    length = bfdev_log_vprint(fmt, para);
    va_end(para);

    return length;
}
