/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <bfdev/fsm.h>

enum {
    TEST_IDLE = 0,
    TEST_CHECK,
    TEST_EXIT,
    TEST_HSTATE,
    TEST_ISTATE,
    TEST_STACK,
    TEST_ASTATE,
};

static int
enter_print(bfdev_fsm_event_t *event, void *data)
{
   printf( "Enter: %s\n", (char *)data);
   return 0;
}

static int
exit_print(bfdev_fsm_event_t *event, void *data)
{
   printf( "Exit: %s\n", (char *)data);
   return 0;
}

static long
trans_guard(bfdev_fsm_event_t *event, const void *cond)
{
    return event->pdata - cond;
}

static int
trans_active(bfdev_fsm_event_t *event, void *data,
             void *curr, void *next)
{
    printf("Active: %s\n", (char *)data);
    return 0;
}

static bfdev_fsm_state_t
test_state[] = {
    [TEST_IDLE] = {
        .parent = &test_state[TEST_CHECK],
        .data = "idle",
        .tnum = 2,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .cond = (void *)(intptr_t)'h',
                .next = &test_state[TEST_HSTATE],
                .guard = trans_guard,
            },
            {
                .cond = (void *)(intptr_t)'e',
                .next = &test_state[TEST_EXIT],
                .guard = trans_guard,
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_HSTATE] = {
        .parent = &test_state[TEST_CHECK],
        .data = "h-state",
        .tnum = 2,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .cond = (void *)(intptr_t)'i',
                .next = &test_state[TEST_ISTATE],
                .guard = trans_guard,
            },
            {
                .cond = (void *)(intptr_t)'a',
                .next = &test_state[TEST_ASTATE],
                .guard = trans_guard,
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_ISTATE] = {
        .parent = &test_state[TEST_CHECK],
        .data = "i-state",
        .tnum = 2,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .cond = (void *)(intptr_t)'\n',
                .next = &test_state[TEST_IDLE],
                .guard = trans_guard,
                .action = trans_active,
                .data = "hello",
            },
            {
                .cond = (void *)(intptr_t)'.',
                .next = &test_state[TEST_STACK],
                .guard = trans_guard,
                .stack = +1,
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_ASTATE] = {
        .parent = &test_state[TEST_CHECK],
        .data = "a-state",
        .tnum = 2,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .cond = (void *)(intptr_t)'\n',
                .next = &test_state[TEST_IDLE],
                .guard = trans_guard,
                .action = trans_active,
                .data = "haha",
            },
            {
                .cond = (void *)(intptr_t)'.',
                .next = &test_state[TEST_STACK],
                .guard = trans_guard,
                .stack = +1,
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_STACK] = {
        .parent = &test_state[TEST_CHECK],
        .data = "stack",
        .tnum = 1,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .action = trans_active,
                .data = "state pop",
                .cross = true,
                .stack = -1,
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_CHECK] = {
        .entry = &test_state[TEST_IDLE],
        .data = "check",
        .tnum = 2,
        .trans = (bfdev_fsm_transition_t[]) {
            {
                .cond = (void *)(intptr_t)'!',
                .next = &test_state[TEST_IDLE],
                .guard = trans_guard,
                .action = trans_active,
                .data = "reset",
            },
            {
                .next = &test_state[TEST_IDLE],
                .action = trans_active,
                .data = "ignore",
            },
        },
        .enter = enter_print,
        .exit = exit_print,
    },
    [TEST_EXIT] = {
        .data = "exit",
        .enter = enter_print,
    },
};

BFDEV_DEFINE_FSM(test_fsm, NULL,
    &test_state[TEST_IDLE],
    &test_state[TEST_EXIT]
);

int main(int argc, const char *argv[])
{
    int ch, retval;

    while ((ch = getc(stdin)) != EOF) {
        retval = bfdev_fsm_handle(&test_fsm,
            &(bfdev_fsm_event_t) {
                .pdata = (void *)(intptr_t)ch,
            }
        );
        if (retval)
            break;
    }

    return 0;
}
