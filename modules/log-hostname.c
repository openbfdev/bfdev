/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
 */

#include <bfdev/log.h>
#include <bfdev/modules/log.h>
#include <sys/utsname.h>
#include <export.h>

static int
log_hook_hostname(bfdev_log_message_t *msg, void *pdata)
{
	struct utsname sys_info;

	if (uname(&sys_info) < 0)
		return -BFDEV_EFAULT;

	return bfdev_msg_append(msg, "%s ", sys_info.nodename);
}

export bfdev_log_chain_t
bfdev_log_chain_hostname = {
	.func = log_hook_hostname,
	.priority = -200,
};

export int
bfdev_log_hostname_enable(bfdev_log_t *log)
{
	return bfdev_log_hook_register(log, &bfdev_log_chain_hostname);
}

export void
bfdev_log_hostname_disable(bfdev_log_t *log)
{
	bfdev_log_hook_unregister(log, &bfdev_log_chain_hostname);
}
