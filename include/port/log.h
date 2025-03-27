/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_LOG_H_
#define _LOCAL_PORT_LOG_H_

#include <base.h>
#include <bfdev/log.h>
#include <port/stdio.h>

extern int
bfport_log_write(bfdev_log_message_t *msg);

#endif /* _LOCAL_PORT_LOG_H_ */
