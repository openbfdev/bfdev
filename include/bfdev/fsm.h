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

typedef struct bfdev_fsm_event bfdev_fsm_event_t;
typedef struct bfdev_fsm_transition bfdev_fsm_transition_t;
typedef struct bfdev_fsm_state bfdev_fsm_state_t;
typedef struct bfdev_fsm bfdev_fsm_t;

typedef int (*bfdev_fsm_trigger_t)
(bfdev_fsm_event_t *event, void *data);

typedef bfdev_fsm_transition_t *(*bfdev_fsm_exception_t)
(bfdev_fsm_event_t *event, void *data);

typedef long (*bfdev_fsm_guard_t)
(bfdev_fsm_event_t *event, const void *cond);

typedef int (*bfdev_fsm_active_t)
(bfdev_fsm_event_t *event, void *data, void *curr, void *next);

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

    const bfdev_fsm_state_t *next;
    bool cross;
    int stack;

    bfdev_fsm_guard_t guard;
    bfdev_fsm_active_t action;
    void *data;
};

struct bfdev_fsm_state {
    bfdev_fsm_trigger_t enter;
    bfdev_fsm_trigger_t exit;
    bfdev_fsm_exception_t exception;
    void *data;

    const bfdev_fsm_transition_t *trans;
    unsigned int tnum;

    const bfdev_fsm_state_t *parent;
    const bfdev_fsm_state_t *entry;
};

struct bfdev_fsm {
    const bfdev_fsm_state_t *state[2];
    const bfdev_fsm_state_t *error;
    bfdev_array_t stack;
    unsigned int count;
};

#define BFDEV_FSM_STATIC(ALLOC, INIT, ERROR) {  \
    .state = {(INIT)}, .error = (ERROR),        \
    .stack = BFDEV_ARRAY_STATIC(                \
        ALLOC, sizeof(bfdev_fsm_state_t *)      \
    ),                                          \
}

#define BFDEV_FSM_INIT(alloc, init, error) \
    (bfdev_fsm_t) BFDEV_FSM_STATIC(alloc, init, error)

#define BFDEV_DEFINE_FSM(name, alloc, init, error) \
    bfdev_fsm_t name = BFDEV_FSM_INIT(alloc, init, error)

static inline void
bfdev_fsm_init(bfdev_fsm_t *fsm, const bfdev_alloc_t *alloc,
               const bfdev_fsm_state_t *init, const bfdev_fsm_state_t *error)
{
    *fsm = BFDEV_FSM_INIT(alloc, init, error);
}

static inline void
bfdev_fsm_reset(bfdev_fsm_t *fsm, const bfdev_fsm_state_t *init)
{
    *fsm->state = init;
    fsm->count = 0;
    bfdev_array_reset(&fsm->stack);
}

static inline const bfdev_fsm_state_t *
bfdev_fsm_curr(bfdev_fsm_t *fsm)
{
    unsigned int count = fsm->count;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

static inline const bfdev_fsm_state_t *
bfdev_fsm_prev(const bfdev_fsm_t *fsm)
{
    unsigned int count = fsm->count - 1;
    return fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)];
}

extern int
bfdev_fsm_error(bfdev_fsm_t *fsm, bfdev_fsm_event_t *event);

extern int
bfdev_fsm_handle(bfdev_fsm_t *fsm, bfdev_fsm_event_t *event);

BFDEV_END_DECLS

#endif /* _BFDEV_FSM_H_ */
