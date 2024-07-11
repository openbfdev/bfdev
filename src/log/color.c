/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
 */

#define COLOR_BLACK     0
#define COLOR_RED       1
#define COLOR_GREEN     2
#define COLOR_YELLOW    3
#define COLOR_BLUE      4
#define COLOR_MAGENTA   5
#define COLOR_CYAN      6
#define COLOR_WHITE     7
#define COLOR_DEFAULT   9

#define COLOR_FG(color) (30 + (color))
#define COLOR_FG_BRIGHT(color) (90 + (color))

static const unsigned int
level_color[] = {
    [BFDEV_LEVEL_EMERG  ] = COLOR_FG_BRIGHT(COLOR_RED),
    [BFDEV_LEVEL_ALERT  ] = COLOR_FG(COLOR_RED),
    [BFDEV_LEVEL_CRIT   ] = COLOR_FG(COLOR_MAGENTA),
    [BFDEV_LEVEL_ERR    ] = COLOR_FG(COLOR_YELLOW),
    [BFDEV_LEVEL_WARNING] = COLOR_FG(COLOR_BLUE),
    [BFDEV_LEVEL_NOTICE ] = COLOR_FG_BRIGHT(COLOR_CYAN),
    [BFDEV_LEVEL_INFO   ] = COLOR_FG_BRIGHT(COLOR_GREEN),
    [BFDEV_LEVEL_DEBUG  ] = COLOR_FG_BRIGHT(COLOR_BLACK),
    [BFDEV_LEVEL_DEFAULT] = COLOR_FG(COLOR_DEFAULT),
};

static void
log_color_prefix(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    if (!bfdev_log_color_test(log))
        return;

    log_scnprintf(msg, "\e[%dm", level_color[msg->level]);
}

static void
log_color_suffix(bfdev_log_t *log, bfdev_log_message_t *msg)
{
    if (!bfdev_log_color_test(log))
        return;

    log_scnprintf(msg, "\e[0m");
}
