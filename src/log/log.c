/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <base.h>
#include "log.h"
#include <bfdev/log.h>
#include <bfdev/scnprintf.h>
#include <port/log.h>
#include <export.h>

export
BFDEV_DEFINE_LOG(
    bfdev_log_default,
    BFDEV_LEVEL_DEFAULT, BFDEV_LEVEL_DEFAULT,
    BFDEV_LOG_COLOR | BFDEV_LOG_LEVEL,
    BFDEV_NULL, BFDEV_NULL
);

export int
bfdev_msg_vappend(bfdev_log_message_t *msg, const char *fmt, bfdev_va_list args)
{
    int append;

    append = bfdev_vscnprintf(msg->buff + msg->length,
        BFDEV_LOG_BUFF_SIZE - msg->length, fmt, args);
    if (bfdev_unlikely(append < 0))
        return -BFDEV_EINVAL;
    msg->length += append;

    return -BFDEV_ENOERR;
}

export int
bfdev_msg_append(bfdev_log_message_t *msg, const char *fmt, ...)
{
    bfdev_va_list args;
    int retval;

    bfdev_va_start(args, fmt);
    retval = bfdev_msg_vappend(msg, fmt, args);
    bfdev_va_end(args);

    return retval;
}

static long
log_chain_priority_cmp(const bfdev_ilist_node_t *node1,
                       const bfdev_ilist_node_t *node2, void *pdata)
{
    int prio1, prio2;

    prio1 = bfdev_container_of(node1, bfdev_log_chain_t, list)->priority;
    prio2 = bfdev_container_of(node2, bfdev_log_chain_t, list)->priority;

    return bfdev_cmp(prio1 > prio2);
}

static inline char
log_get_level(const char *str)
{
    if (str[0] == BFDEV_SOH_ASCII && str[1])
        return str[1];

    return 0;
}

export unsigned int
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

static bfdev_ilist_head_t *
log_priority_chain(bfdev_log_t *log, int priority)
{
    return priority <= 0 ? &log->prefix_chain : &log->suffix_chain;
}

static int
log_call_chain(bfdev_log_message_t *msg, bfdev_ilist_head_t *chain)
{
    bfdev_log_chain_t *node, *tmp;
    int retval;

    bfdev_ilist_for_each_entry_safe(node, tmp, chain, list) {
        retval = node->func(msg, node->pdata);
        if (retval < 0)
            return retval;
    }

    return -BFDEV_ENOERR;
}

static int
log_prefix_fixed(bfdev_log_message_t *msg)
{
    bfdev_log_t *log;
    int retval;

    log = msg->log;
    if (bfdev_log_level_test(log)) {
        retval = log_level_prefix(log, msg);
        if (bfdev_unlikely(retval))
            return retval;
    }

    if (bfdev_log_color_test(log)) {
        retval = log_color_prefix(log, msg);
        if (bfdev_unlikely(retval))
            return retval;
    }

    return -BFDEV_ENOERR;
}

static int
log_suffix_fixed(bfdev_log_message_t *msg)
{
    bfdev_log_t *log;
    int retval;

    log = msg->log;
    if (bfdev_log_color_test(log)) {
        retval = log_color_suffix(log, msg);
        if (bfdev_unlikely(retval))
            return retval;
    }

    return -BFDEV_ENOERR;
}

static int
log_prefix(bfdev_log_message_t *msg)
{
    bfdev_log_t *log;
    int retval;

    log = msg->log;
    retval = log_call_chain(msg, &log->prefix_chain);
    if (bfdev_unlikely(retval))
        return retval;

    retval = log_prefix_fixed(msg);
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

static int
log_suffix(bfdev_log_message_t *msg)
{
    bfdev_log_t *log;
    int retval;

    log = msg->log;
    retval = log_suffix_fixed(msg);
    if (bfdev_unlikely(retval))
        return retval;

    retval = log_call_chain(msg, &log->suffix_chain);
    if (bfdev_unlikely(retval))
        return retval;

    return -BFDEV_ENOERR;
}

static int
log_write(bfdev_log_message_t *msg)
{
    bfdev_log_t *log;

    log = msg->log;
    if (log->write)
        return log->write(msg);

    return bfport_log_write(msg);
}

static int
log_emit(bfdev_log_t *log, unsigned int level, const char *fmt, bfdev_va_list args)
{
    char buff[BFDEV_LOG_BUFF_SIZE];
    bfdev_log_message_t msg;
    int retval;

    if (!log)
        log = &bfdev_log_default;

    if (level >= BFDEV_LEVEL_DEFAULT)
        level = log->default_level;

    if (level > log->record_level)
        return -BFDEV_ENOERR;

    msg.log = log;
    msg.level = level;
    msg.buff = buff;
    msg.length = 0;

    retval = log_prefix(&msg);
    if (bfdev_unlikely(retval))
        return retval;

    retval = bfdev_msg_vappend(&msg, fmt, args);
    if (bfdev_unlikely(retval))
        return retval;

    retval = log_suffix(&msg);
    if (bfdev_unlikely(retval))
        return retval;

    retval = log_write(&msg);
    if (bfdev_unlikely(retval < 0))
        return -BFDEV_EIO;

    return -BFDEV_ENOERR;
}

export int
bfdev_vlog_core(bfdev_log_t *log, const char *fmt, bfdev_va_list args)
{
    unsigned int level;

    level = bfdev_log_level(fmt, &fmt);

    return log_emit(log, level, fmt, args);
}

export int
bfdev_log_core(bfdev_log_t *log, const char *fmt, ...)
{
    bfdev_va_list para;
    int length;

    bfdev_va_start(para, fmt);
    length = bfdev_vlog_core(log, fmt, para);
    bfdev_va_end(para);

    return length;
}

export int
bfdev_log_chain_register(bfdev_log_t *log, bfdev_log_chain_t *hook)
{
    bfdev_ilist_head_t *chain;

    if (bfdev_unlikely(!hook->func))
        return -BFDEV_EINVAL;

    chain = log_priority_chain(log, hook->priority);
    bfdev_ilist_node_init(&hook->list);
    bfdev_ilist_add(chain, &hook->list, log_chain_priority_cmp, BFDEV_NULL);

    return -BFDEV_ENOERR;
}

export void
bfdev_log_chain_unregister(bfdev_log_t *log, bfdev_log_chain_t *hook)
{
    bfdev_ilist_head_t *chain;

    chain = log_priority_chain(log, hook->priority);
    bfdev_ilist_del(chain, &hook->list);
}
