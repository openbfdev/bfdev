/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_FSM_H_
#define _BFDEV_FSM_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/macro.h>

BFDEV_BEGIN_DECLS

#ifndef BFDEV_FSM_STACK
# define BFDEV_FSM_STACK 32
#endif

struct bfdev_fsm_event;
struct bfdev_fsm_state;

typedef long (*bfdev_fsm_guard_t)
(struct bfdev_fsm_event *event, const void *cond);

typedef int (*bfdev_fsm_event_t)
(struct bfdev_fsm_event *event, void *data);

typedef int (*bfdev_fsm_active_t)
(struct bfdev_fsm_event *event, void *data, void *curr, void *next);

struct bfdev_fsm_event {
    unsigned int type;
    void *pdata;
};

struct bfdev_fsm_transition {
    unsigned int type;
    const void *cond;
    struct bfdev_fsm_state *next;

    bfdev_fsm_guard_t guard;
    bfdev_fsm_active_t action;
    void *data;
};

struct bfdev_fsm_state {
    bfdev_fsm_event_t enter;
    bfdev_fsm_event_t exit;
    void *data;

    struct bfdev_fsm_transition *trans;
    struct bfdev_fsm_state *parent;
    struct bfdev_fsm_state *entry;
};

struct bfdev_fsm {
    struct bfdev_fsm_state *state[2];
    struct bfdev_fsm_state *error;
    unsigned int count;
};

#define BFDEV_FSM_STATIC(INIT, ERROR) \
    {.state = {(INIT)}, .error = (ERROR)}

#define BFDEV_FSM_INIT(init, error) \
    (struct bfdev_fsm) BFDEV_FSM_STATIC(init, error)

#define BFDEV_DEFINE_FSM(name, init, error) \
    struct bfdev_fsm name = BFDEV_FSM_INIT(init, error)

static inline void
bfdev_fsm_init(struct bfdev_fsm *fsm, struct bfdev_fsm_state *init,
               struct bfdev_fsm_state *error)
{
    *fsm = BFDEV_FSM_INIT(init, error);
}

static inline struct bfdev_fsm_state *
bfdev_fsm_prev(struct bfdev_fsm *fsm)
{
    unsigned int count = fsm->count - 1;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

static inline struct bfdev_fsm_state *
bfdev_fsm_curr(struct bfdev_fsm *fsm)
{
    unsigned int count = fsm->count - 0;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

extern int
bfdev_fsm_handle(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event);

BFDEV_END_DECLS

#endif /* _BFDEV_FSM_H */
