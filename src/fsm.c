/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/fsm.h>
#include <export.h>

static inline void
fsm_push_state(struct bfdev_fsm *fsm, const struct bfdev_fsm_state *state)
{
    unsigned int count = ++fsm->count;
    fsm->state[count & (BFDEV_ARRAY_SIZE(fsm->state) - 1)] = state;
}

static const struct bfdev_fsm_transition *
fsm_find_transition(const struct bfdev_fsm_state *state, struct bfdev_fsm_event *event)
{
    const struct bfdev_fsm_transition *find;
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

export int
bfdev_fsm_error(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event)
{
    const struct bfdev_fsm_state *error = fsm->error;
    int retval = 0;

    fsm_push_state(fsm, error);
    if (error && error->enter)
        retval = error->enter(event, error->data);

    return retval;
}

export int
bfdev_fsm_handle(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event)
{
    const struct bfdev_fsm_state *curr = bfdev_fsm_curr(fsm);
    const struct bfdev_fsm_state *next;

    if (bfdev_unlikely(!curr))
        return -BFDEV_EINVAL;

    for (next = curr; next; curr = next) {
        const struct bfdev_fsm_transition *tran;
        const struct bfdev_fsm_state **pstate;
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

        if (bfdev_unlikely(!next)) {
            retval = bfdev_fsm_error(fsm, event);
            return retval ?: -BFDEV_ENOENT;
        }

        while (next->entry)
            next = next->entry;

        if (curr != next && tran->stack <= 0 && curr->exit) {
            retval = curr->exit(event, curr->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        if (tran->action) {
            retval = tran->action(event, tran->data, curr->data, next->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        if (curr != next && tran->stack >= 0 && next->enter) {
            retval = next->enter(event, next->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        fsm_push_state(fsm, next);
        if (bfdev_unlikely(next == fsm->error))
            return -BFDEV_EFAULT;

        if (curr == next)
            return BFDEV_FSM_SELFLOOP;

        if (!next->tnum)
            return BFDEV_FSM_FINISH;

        if (tran->stack > 0) {
            pstate = bfdev_array_push(&fsm->stack, 1);
            if (bfdev_unlikely(!pstate))
                return -BFDEV_ENOMEM;
            *pstate = curr;
        }

        if (!tran->cross)
            return BFDEV_FSM_CHANGED;
    }

    return BFDEV_FSM_NOCHANGE;
}
