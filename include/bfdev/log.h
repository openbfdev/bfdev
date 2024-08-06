/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LOG_H_
#define _BFDEV_LOG_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/stdarg.h>
#include <bfdev/level.h>
#include <bfdev/bits.h>
#include <bfdev/bitflags.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_LOG_BUFF_SIZE
# define BFDEV_LOG_BUFF_SIZE 512
#endif

#ifndef BFDEV_LOGLEVEL_MAX
# define BFDEV_LOGLEVEL_MAX BFDEV_LEVEL_DEBUG
#endif

typedef struct bfdev_log bfdev_log_t;
typedef struct bfdev_log_message bfdev_log_message_t;

typedef int (*bfdev_log_write_t)
(bfdev_log_message_t *msg, void *pdata);

enum bfdev_log_flags {
    __BFDEV_LOG_COLOR = 0,
    __BFDEV_LOG_LEVEL,

    BFDEV_LOG_COLOR = BFDEV_BIT(__BFDEV_LOG_COLOR),
    BFDEV_LOG_LEVEL = BFDEV_BIT(__BFDEV_LOG_LEVEL),
};

struct bfdev_log {
    unsigned int default_level;
    unsigned int record_level;
    unsigned long flags;

    bfdev_log_write_t write;
    void *pdata;
};

struct bfdev_log_message {
    unsigned int level;
    char *buff;
    size_t length;
};

#define BFDEV_LOG_STATIC(HEAD, DEFAULT, RECORD, FLAGS, WRITE, PDATA) { \
    .default_level = (DEFAULT), .record_level = (RECORD), \
    .flags = (FLAGS), .write = (WRITE), .pdata = (PDATA), \
}

#define BFDEV_LOG_INIT(head, default, record, flags, write, pdata) \
    (bfdev_log_t) BFDEV_LOG_STATIC(head, default, record, flags, write, pdata)

#define BFDEV_DEFINE_LOG(name, default, record, flags, write, pdata) \
    bfdev_log_t name = BFDEV_LOG_INIT(&name, default, record, flags, write, pdata)

BFDEV_BITFLAGS_STRUCT(
    bfdev_log_color,
    struct bfdev_log, flags,
    __BFDEV_LOG_COLOR
)

BFDEV_BITFLAGS_STRUCT(
    bfdev_log_level,
    struct bfdev_log, flags,
    __BFDEV_LOG_LEVEL
)

extern bfdev_log_t
bfdev_log_default;

static inline void
bfdev_log_init(bfdev_log_t *log, unsigned int def, unsigned int record,
               unsigned long flags, bfdev_log_write_t write, void *pdata)
{
    *log = BFDEV_LOG_INIT(log, def, record, flags, write, pdata);
}

extern unsigned int
bfdev_log_level(const char *str, const char **endptr);

extern __bfdev_printf(2, 0) int
bfdev_vlog_core(bfdev_log_t *log, const char *fmt, va_list args);

extern __bfdev_printf(2, 3) int
bfdev_log_core(bfdev_log_t *log, const char *fmt, ...);

/**
 * bfdev_log_fmt - used by the bfdev_log_*() macros to generate the bfdev_log format string
 * @fmt: format string passed from a bfdev_log_*() macro
 *
 * This macro can be used to generate a unified format string for bfdev_log_*()
 * macros. A common use is to prefix all bfdev_log_*() messages in a file with a common
 * string. For example, defining this at the top of a source file:
 *
 *        #define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt
 *
 * would prefix all bfdev_log_info, bfdev_log_emerg... messages in the file with the module
 * name.
 */
#ifndef bfdev_log_fmt
# define bfdev_log_fmt(fmt) fmt
#endif

#ifndef bfdev_log_conf
# define bfdev_log_conf NULL
#endif

#define bfdev_vlog(fmt, args) \
    bfdev_vlog_core(bfdev_log_conf, fmt, args)

#define bfdev_log(fmt, ...) \
    bfdev_log_core(bfdev_log_conf, fmt, ##__VA_ARGS__)

/*
 * Dummy bfdev_log for disabled debugging statements to use whilst maintaining
 * gcc's format checking.
 */
#define bfdev_log_none(fmt, ...) ({     \
    if (0)                              \
        bfdev_log(fmt, ##__VA_ARGS__);  \
    0;                                  \
})

/**
 * bfdev_log_emerg - Print an emergency-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_EMERG loglevel.
 * It uses bfdev_log_fmt() to generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_EMERG
# define bfdev_log_emerg(fmt, ...) \
    bfdev_log(BFDEV_EMERG bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_emerg(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_alert - Print an alert-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_ALERT loglevel.
 * It uses bfdev_log_fmt() to generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_ALERT
# define bfdev_log_alert(fmt, ...) \
    bfdev_log(BFDEV_ALERT bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_alert(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_crit - Print a critical-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_CRIT loglevel.
 * It uses bfdev_log_fmt() to generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_CRIT
# define bfdev_log_crit(fmt, ...) \
    bfdev_log(BFDEV_CRIT bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_crit(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_err - Print an error-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_ERR loglevel.
 * It uses bfdev_log_fmt() to generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_ERR
# define bfdev_log_err(fmt, ...) \
    bfdev_log(BFDEV_ERR bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_err(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_warn - Print a warning-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_WARNING loglevel.
 * It uses bfdev_log_fmt() to generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_WARNING
# define bfdev_log_warn(fmt, ...) \
    bfdev_log(BFDEV_WARNING bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_warn(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_notice - Print a notice-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_NOTICE loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_NOTICE
# define bfdev_log_notice(fmt, ...) \
    bfdev_log(BFDEV_NOTICE bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_notice(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_info - Print an info-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_INFO loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_INFO
# define bfdev_log_info(fmt, ...) \
    bfdev_log(BFDEV_INFO bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_info(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

/**
 * bfdev_log_debug - Print an debug-level message
 * @fmt: format string
 * @...: arguments for the format string
 *
 * This macro expands to a bfdev_log with BFDEV_DEBUG loglevel. It uses bfdev_log_fmt() to
 * generate the format string.
 */
#if BFDEV_LOGLEVEL_MAX >= BFDEV_LEVEL_DEBUG
# define bfdev_log_debug(fmt, ...) \
    bfdev_log(BFDEV_DEBUG bfdev_log_fmt(fmt), ##__VA_ARGS__)
#else
# define bfdev_log_debug(fmt, ...) \
    bfdev_log_none(fmt, ##__VA_ARGS__)
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_LOG_H_ */
