/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MODULES_LOG_H_
#define _BFDEV_MODULES_LOG_H_

#include <bfdev/config.h>
#include <bfdev/log.h>

BFDEV_BEGIN_DECLS

extern bfdev_log_chain_t
bfdev_log_chain_hostname;

extern bfdev_log_chain_t
bfdev_log_chain_localtime;

extern bfdev_log_chain_t
bfdev_log_chain_pid;

extern int
bfdev_log_localtime_enable(bfdev_log_t *log);

extern void
bfdev_log_localtime_disable(bfdev_log_t *log);

extern int
bfdev_log_hostname_enable(bfdev_log_t *log);

extern void
bfdev_log_hostname_disable(bfdev_log_t *log);

extern int
bfdev_log_pid_enable(bfdev_log_t *log);

extern void
bfdev_log_pid_disable(bfdev_log_t *log);

BFDEV_END_DECLS

#endif /* _BFDEV_TEMPLATE_BTREE_H_ */
