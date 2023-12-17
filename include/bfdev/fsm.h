/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_FSM_H_
#define _BFDEV_FSM_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/macro.h>
#include <bfdev/array.h>

BFDEV_BEGIN_DECLS

struct bfdev_fsm_event;
struct bfdev_fsm_transition;
struct bfdev_fsm_state;

typedef int (*bfdev_fsm_event_t)
(struct bfdev_fsm_event *event, void *data);

typedef struct bfdev_fsm_transition *(*bfdev_fsm_exception_t)
(struct bfdev_fsm_event *event, void *data);

typedef long (*bfdev_fsm_guard_t)
(struct bfdev_fsm_event *event, const void *cond);

typedef int (*bfdev_fsm_active_t)
(struct bfdev_fsm_event *event, void *data, void *curr, void *next);

enum bfdev_fsm_retval {
    BFDEV_FSM_CHANGED = 0,
    BFDEV_FSM_NOCHANGE,
    BFDEV_FSM_SELFLOOP,
    BFDEV_FSM_FINISH,
};

struct bfdev_fsm_event {
    unsigned int type;
    void *pdata;
};

struct bfdev_fsm_transition {
    unsigned int type;
    const void *cond;

    const struct bfdev_fsm_state *next;
    bool cross;
    int stack;

    bfdev_fsm_guard_t guard;
    bfdev_fsm_active_t action;
    void *data;
};

struct bfdev_fsm_state {
    bfdev_fsm_event_t enter;
    bfdev_fsm_event_t exit;
    bfdev_fsm_exception_t exception;
    void *data;

    const struct bfdev_fsm_transition *trans;
    unsigned int tnum;

    const struct bfdev_fsm_state *parent;
    const struct bfdev_fsm_state *entry;
};

struct bfdev_fsm {
    const struct bfdev_fsm_state *state[2];
    const struct bfdev_fsm_state *error;
    struct bfdev_array stack;
    unsigned int count;
};

#define BFDEV_FSM_STATIC(ALLOC, INIT, ERROR) {  \
    .state = {(INIT)}, .error = (ERROR),        \
    .stack = BFDEV_ARRAY_STATIC(                \
        ALLOC, sizeof(struct bfdev_fsm_state *) \
    ),                                          \
}

#define BFDEV_FSM_INIT(alloc, init, error) \
    (struct bfdev_fsm) BFDEV_FSM_STATIC(alloc, init, error)

#define BFDEV_DEFINE_FSM(name, alloc, init, error) \
    struct bfdev_fsm name = BFDEV_FSM_INIT(alloc, init, error)

static inline void
bfdev_fsm_init(struct bfdev_fsm *fsm, const struct bfdev_alloc *alloc,
               const struct bfdev_fsm_state *init, const struct bfdev_fsm_state *error)
{
    *fsm = BFDEV_FSM_INIT(alloc, init, error);
}

static inline void
bfdev_fsm_reset(struct bfdev_fsm *fsm, const struct bfdev_fsm_state *init)
{
    *fsm->state = init;
    fsm->count = 0;
    bfdev_array_reset(&fsm->stack);
}

static inline const struct bfdev_fsm_state *
bfdev_fsm_curr(struct bfdev_fsm *fsm)
{
    unsigned int count = fsm->count;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

static inline const struct bfdev_fsm_state *
bfdev_fsm_prev(const struct bfdev_fsm *fsm)
{
    unsigned int count = fsm->count - 1;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

extern int
bfdev_fsm_error(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event);

extern int
bfdev_fsm_handle(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event);

BFDEV_END_DECLS

#endif /* _BFDEV_FSM_H */
