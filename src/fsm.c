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

    if (!state->trans)
        return NULL;

    for (count = 0; state->trans[count].next || state->trans[count].stack; ++count) {
        find = &state->trans[count];

        if (find->type != event->type)
            continue;

        if (!find->guard || !find->guard(event, find->cond))
            return find;
    }

    return NULL;
}

export int
bfdev_fsm_handle(struct bfdev_fsm *fsm, struct bfdev_fsm_event *event)
{
    struct bfdev_fsm_state *curr = bfdev_fsm_curr(fsm);
    struct bfdev_fsm_state *next = curr;

    if (unlikely(!curr))
        return -BFDEV_EINVAL;

    do {
        struct bfdev_fsm_transition *tran;
        struct bfdev_fsm_state **pstate;
        int retval;

        tran = fsm_find_transition(next, event);
        if (!tran) {
            next = next->parent;
            continue;
        }

        if ((next = tran->next)) {
            while (next->entry)
                next = next->entry;
        } else {
            pstate = bfdev_array_pop(&fsm->stack, -tran->stack);
            next = *pstate;
        }

        if (next != curr) {
            if (curr->exit) {
                retval = curr->exit(event, curr->data);
                if (unlikely(retval))
                    return retval;
            }

            if (tran->action) {
                retval = tran->action(event, tran->data, curr, next);
                if (unlikely(retval))
                    return retval;
            }

            if (next->enter) {
                retval = curr->enter(event, next->data);
                if (unlikely(retval))
                    return retval;
            }
        }

        if (tran->stack > 0) {
            pstate = bfdev_array_push(&fsm->stack, 1);
            if (unlikely(!pstate))
                return -BFDEV_ENOMEM;
            *pstate = curr;
        }

        fsm_push_state(fsm, next);
        if (tran->cross)
            continue;

        if (unlikely(curr == next))
            return -BFDEV_EDEADLK;

        if (unlikely(next == fsm->error))
            return -BFDEV_EFAULT;

        return -BFDEV_ENOERR;
    } while ((curr = next));

    return -BFDEV_ENOENT;
}
