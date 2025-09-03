/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_MODULES_LOG_H_
#define _BFDEV_MODULES_LOG_H_

#include <bfdev/config.h>
#include <bfdev/log.h>

BFDEV_BEGIN_DECLS

extern int
bfdev_log_chain_hostname(bfdev_log_message_t *msg, void *pdata);

extern int
bfdev_log_chain_localtime(bfdev_log_message_t *msg, void *pdata);

extern int
bfdev_log_chain_pid(bfdev_log_message_t *msg, void *pdata);

extern int
bfdev_log_write_syslog(bfdev_log_message_t *msg);

BFDEV_END_DECLS

#endif /* _BFDEV_TEMPLATE_BTREE_H_ */
