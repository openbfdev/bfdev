/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/fsm.h>
#include <export.h>

static inline void
fsm_push_state(bfdev_fsm_t *fsm, const bfdev_fsm_state_t *state)
{
    unsigned int count, index;

    count = ++fsm->count;
    index = count & (BFDEV_ARRAY_SIZE(fsm->state) - 1);

    fsm->state[index] = state;
}

static const bfdev_fsm_transition_t *
fsm_find_transition(const bfdev_fsm_state_t *state, bfdev_fsm_event_t *event)
{
    const bfdev_fsm_transition_t *find;
    unsigned int count;

    for (count = 0; count < state->tnum; ++count) {
        find = &state->trans[count];
        if (bfdev_fsm_cond(find, event))
            return find;
    }

    /* No transition conditions found, triggering exception */
    if (state->exception)
        return state->exception(event, state->data);

    return NULL;
}

export int
bfdev_fsm_error(bfdev_fsm_t *fsm, bfdev_fsm_event_t *event)
{
    const bfdev_fsm_state_t *error;
    int retval;

    error = fsm->error;
    retval = 0;

    fsm_push_state(fsm, error);
    if (error && error->enter)
        retval = error->enter(event, error->data);

    return retval;
}

export int
bfdev_fsm_handle(bfdev_fsm_t *fsm, bfdev_fsm_event_t *event)
{
    const bfdev_fsm_state_t *curr, *next;

    curr = bfdev_fsm_curr(fsm);
    if (bfdev_unlikely(!curr))
        return -BFDEV_EINVAL;

    for (next = curr; next; curr = next) {
        const bfdev_fsm_transition_t *tran;
        const bfdev_fsm_state_t **pstate;
        int retval;

        /*
         * If there were no transitions for the given event for
         * the current state, check if there are any transitions
         * for any of the parent states (if any).
         */
        tran = fsm_find_transition(curr, event);
        if (!tran) {
            next = curr->parent;
            continue;
        }

        /*
         * Pop the stack as the new state, if popping is required
         * and there have no next state.
         */
        next = tran->next;
        if (!next && tran->stack < 0) {
            pstate = bfdev_array_pop(&fsm->stack, -tran->stack);
            if (bfdev_unlikely(!pstate))
                return -BFDEV_EOVERFLOW;
            next = *pstate;
        }

        /*
         * A transition must have a next state defined.
         * If the user has not defined the next state,
         * go to error state.
         */
        if (bfdev_unlikely(!next)) {
            retval = bfdev_fsm_error(fsm, event);
            return retval ?: -BFDEV_ENOENT;
        }

        /*
         * If the new state is a parent state, enter its entry
         * state (if it has one). Step down through the whole family
         * tree until a state without an entry state is found.
         */
        while (next->entry)
            next = next->entry;

        /*
         * Call the curr state's exit action, if state
         * does not return to itself and the stack
         * has not popped.
         */
        if (curr != next && tran->stack <= 0 && curr->exit) {
            retval = curr->exit(event, curr->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        /* Run transition action */
        if (tran->action) {
            retval = tran->action(event, tran->data, curr->data, next->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        /*
         * Call the new state's entry action, if state
         * does not return to itself and has not been
         * pushed onto the stack.
         */
        if (curr != next && tran->stack >= 0 && next->enter) {
            retval = next->enter(event, next->data);
            if (bfdev_unlikely(retval))
                return retval;
        }

        /* Update history state */
        fsm_push_state(fsm, next);
        if (bfdev_unlikely(next == fsm->error))
            return -BFDEV_EFAULT;

        /* State returned to itself */
        if (curr == next)
            return BFDEV_FSM_SELFLOOP;

        /*
         * If the new state is a final state, notify user
         * that the state machine has stopped
         */
        if (!next->tnum && !next->exception)
            return BFDEV_FSM_FINISH;

        /*
         * If it needs to be pushed onto the stack,
         * push the curr state.
         */
        if (tran->stack > 0) {
            pstate = bfdev_array_push(&fsm->stack, 1);
            if (bfdev_unlikely(!pstate))
                return -BFDEV_ENOMEM;
            *pstate = curr;
        }

        /*
         * If it is declared as crossing, Immediately
         * change to the next state.
         */
        if (tran->cross)
            continue;

        return BFDEV_FSM_CHANGED;
    }

    return BFDEV_FSM_NOCHANGE;
}
