/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/log.h>
#include <bfdev/modules/log.h>
#include <pthread.h>
#include <unistd.h>
#include <export.h>

static int
log_hook_pid(bfdev_log_message_t *msg, void *pdata)
{
	return bfdev_msg_append(msg, "(pid: %d) ", getpid());
}

export bfdev_log_chain_t
bfdev_log_chain_pid = {
	.func = log_hook_pid,
	.priority = -100,
};

export int
bfdev_log_pid_enable(bfdev_log_t *log)
{
	return bfdev_log_hook_register(log, &bfdev_log_chain_pid);
}

export void
bfdev_log_pid_disable(bfdev_log_t *log)
{
	bfdev_log_hook_unregister(log, &bfdev_log_chain_pid);
}
