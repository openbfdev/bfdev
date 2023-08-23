/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/fsm.h>
#include <export.h>

static inline void
fsm_push_state(struct bfdev_fsm *fsm, struct bfdev_fsm_state *state)
{
    unsigned int count = ++fsm->count;
    fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)] = state;
}

static struct bfdev_fsm_transition *
fsm_find_transition(struct bfdev_fsm_state *state, struct bfdev_fsm_event *event)
{
    struct bfdev_fsm_transition *find;
    unsigned int count;

    for (count = 0; count < state->tnum; ++count) {
        find = &state->trans[count];

        if (find->type != event->type)
            continue;

        if (!find->guard || !find->guard(event, find->cond))
            return find;
    }

    return NULL;
}

static int
fsm_error(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event)
{
    struct bfdev_fsm_state *error = fsm->error;
    int retval = 0;

    fsm_push_state(fsm, error);
    if (error && error->enter)
        retval = error->enter(event, error->data);

    return retval;
}

export int
bfdev_fsm_handle(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event)
{
    struct bfdev_fsm_state *curr = bfdev_fsm_curr(fsm);
    struct bfdev_fsm_state *next;

    if (unlikely(!curr))
        return -BFDEV_EINVAL;

    for (next = curr; next; curr = next) {
        struct bfdev_fsm_transition *tran;
        struct bfdev_fsm_state **pstate;
        int retval;

        tran = fsm_find_transition(curr, event);
        if (!tran) {
            next = curr->parent;
            continue;
        }

        if (!(next = tran->next) && tran->stack < 0) {
            pstate = bfdev_array_pop(&fsm->stack, -tran->stack);
            next = *pstate;
        }

        if (unlikely(!next)) {
            retval = fsm_error(fsm, event);
            return retval ?: -BFDEV_ENOENT;
        }

        while (next->entry)
            next = next->entry;

        if (next != curr && curr->exit) {
            retval = curr->exit(event, curr->data);
            if (unlikely(retval))
                return retval;
        }

        if (tran->action) {
            retval = tran->action(event, tran->data, curr, next);
            if (unlikely(retval))
                return retval;
        }

        if (next != curr && next->enter) {
            retval = next->enter(event, next->data);
            if (unlikely(retval))
                return retval;
        }

        if (tran->stack > 0) {
            pstate = bfdev_array_push(&fsm->stack, 1);
            if (unlikely(!pstate))
                return -BFDEV_ENOMEM;
            *pstate = curr;
        }

        fsm_push_state(fsm, next);
        if (unlikely(next == fsm->error))
            return -BFDEV_EFAULT;

        if (tran->cross)
            continue;

        if (unlikely(curr == next))
            return BFDEV_FSM_SELFLOOP;

        if (!next->tnum)
            return BFDEV_FSM_FINISH;

        return BFDEV_FSM_CHANGED;
    }

    return BFDEV_FSM_NOCHANGE;
}
