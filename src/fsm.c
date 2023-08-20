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

    for (count = 0; state->trans[count].next; ++count) {
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

        tran = fsm_find_transition(next, event);
        if (!tran) {
            next = next->parent;
            continue;
        }

        next = tran->next;
        while (next->entry)
            next = next->entry;

        if (next != curr) {
            if (curr->exit)
                curr->exit(event, curr->data);

            if (tran->action)
                tran->action(event, tran->data, curr, next);

            if (next->enter)
                curr->enter(event, next->data);
        }

        fsm_push_state(fsm, next);
        if (unlikely(curr == next))
            return -BFDEV_EDEADLK;

        if (unlikely(next == fsm->error))
            return -BFDEV_EFAULT;

        return -BFDEV_ENOERR;
    } while ((curr = next));

    return -BFDEV_ENOENT;
}
