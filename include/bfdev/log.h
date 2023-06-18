/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LOG_H_
#define _BFDEV_LOG_H_

#include <bfdev/config.h>
#include <bfdev/stdarg.h>
#include <bfdev/level.h>

BFDEV_BEGIN_DECLS

extern unsigned int bfdev_log_level(const char *str, const char **endptr);
extern __bfdev_printf(1, 0) int bfdev_log_vprint(const char *fmt, va_list args);
extern __bfdev_printf(1, 2) int bfdev_log_print(const char *fmt, ...);

/**
 * bfdev_log_fmt - used by the bfdev_log_*() macros to generate the printk format string
 * @fmt: format string passed from a bfdev_log_*() macro
 *
 * This macro can be used to generate a unified format string for bfdev_log_*()
 * macros. A common use is to prefix all bfdev_log_*() messages in a file with a common
 * string. For example, defining this at the top of a source file:
 *
 *        #define bfdev_log_fmt(fmt) KBUILD_MODNAME ": " fmt
 *
 * would prefix all bfdev_log_info, bfdev_log_emerg... messages in the file with the module
 * name.
 */
#ifndef bfdev_log_fmt
# define bfdev_log_fmt(fmt) fmt
#endif

/*
 * Dummy printk for disabled debugging statements to use whilst maintaining
 * gcc's format checking.
 */
# define bfdev_log_none(fmt, ...) ({            \
    if (0)                              \
        bfdev_log_print(fmt, ##__VA_ARGS__);     \
    0;                                  \
})

/**
 * bfdev_log_emerg - Print an emergency-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_EMERG loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_EMERG
# define bfdev_log_emerg(fmt, ...) \
    bfdev_log_print(BFDEV_EMERG bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_emerg(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_alert - Print an alert-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_ALERT loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_ALERT
# define bfdev_log_alert(fmt, ...) \
    bfdev_log_print(BFDEV_ALERT bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_alert(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_crit - Print a critical-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_CRIT loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_CRIT
# define bfdev_log_crit(fmt, ...) \
    bfdev_log_print(BFDEV_CRIT bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_crit(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_err - Print an error-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_ERR loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_ERR
# define bfdev_log_err(fmt, ...) \
    bfdev_log_print(BFDEV_ERR bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_err(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_warn - Print a warning-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_WARNING loglevel. It uses bfdev_log_fmt()
 * to generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_WARNING
# define bfdev_log_warn(fmt, ...) \
    bfdev_log_print(BFDEV_WARNING bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_warn(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_notice - Print a notice-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_NOTICE loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_NOTICE
# define bfdev_log_notice(fmt, ...) \
    bfdev_log_print(BFDEV_NOTICE bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_notice(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_info - Print an info-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_INFO loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_INFO
# define bfdev_log_info(fmt, ...) \
    bfdev_log_print(BFDEV_INFO bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_info(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_debug - Print an debug-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a printk with BFDEV_DEBUG loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if CONFIG_PRINTK_LOGLEVEL_MAX >= KLEVEL_INFO
# define bfdev_log_debug(fmt, ...) \
    bfdev_log_print(BFDEV_DEBUG bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_debug(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_LOG_H_ */
