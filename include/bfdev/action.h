/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ACTION_H_
#define _BFDEV_ACTION_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/errno.h>

BFDEV_BEGIN_DECLS

typedef int
(*bfdev_action_func_t)(void *data);

struct bfdev_action {
    bfdev_action_func_t func;
    const void *data;
};

#define BFDEV_ACTION_STATIC(FUNC, DATA) { \
    .func = (FUNC), .data = (DATA), \
}

#define BFDEV_ACTION_INIT(func, data) \
    (struct bfdev_action) BFDEV_ACTION_STATIC(func, data)

#define BFDEV_DEFINE_ACTION(name, func, data) \
    struct bfdev_action name = BFDEV_ACTION_INIT(func, data)

/**
 * bfdev_action_init - initialize action.
 * @action: the action pointer.
 * @func: function to callback.
 * @data: callback data of @func.
 */
static inline void
bfdev_action_init(struct bfdev_action *action,
                  bfdev_action_func_t func, const void *data)
{
    *action = BFDEV_ACTION_INIT(func, data);
}

/**
 * bfdev_action_update - update action data.
 * @action: the action pointer.
 * @data: the data to update.
 */
static inline void
bfdev_action_update(struct bfdev_action *action, const void *data)
{
    action->data = data;
}

/**
 * bfdev_action_clear - clear action callback.
 * @action: the action pointer.
 */
static inline void
bfdev_action_clear(struct bfdev_action *action)
{
    action->func = NULL;
}

/**
 * bfdev_action_call - callback an action.
 * @action: the action pointer.
 */
static inline int
bfdev_action_call(struct bfdev_action *action)
{
    if (bfdev_likely(action->func))
        return action->func((void *)action->data);
    return -BFDEV_ENODATA;
}

BFDEV_END_DECLS

#endif /* _BFDEV_ACTION_H_ */
