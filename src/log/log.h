/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_LOG_H_
#define _LOCAL_LOG_H_

#include <base.h>
#include <bfdev/log.h>

extern int
log_level_prefix(bfdev_log_t *log, bfdev_log_message_t *msg);

extern int
log_color_prefix(bfdev_log_t *log, bfdev_log_message_t *msg);

extern int
log_color_suffix(bfdev_log_t *log, bfdev_log_message_t *msg);

#endif /* _LOCAL_LOG_H_ */
