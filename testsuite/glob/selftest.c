/* SPDX-License-Identifier GPL-2.0-or-later */
/*
 * Copyright(c) 2022-2024 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "bitwalk-selftest"
#define bfdev_log_fmt(fmt) MODULE_NAME ":" fmt

#include <stddef.h>
#include <stdbool.h>
#include <bfdev/glob.h>
#include <bfdev/macro.h>
#include <bfdev/log.h>
#include <testsuite.h>

struct glob_test {
    const char *glob;
    const char *string;
    bool result;
};

static const struct glob_test
test_glob[] = {
    /* Some basic tests */
    {"a", "a",   true},
    {"a", "b",  false},
    {"a", "aa", false},
    {"a", "",   false},
    {"",  "",    true},
    {"",  "a",  false},

    /* Simple character class tests */
    {"[a]",   "a",  true},
    {"[a]",   "b", false},
    {"[!a]",  "a", false},
    {"[!a]",  "b",  true},
    {"[ab]",  "a",  true},
    {"[ab]",  "b",  true},
    {"[ab]",  "c", false},
    {"[!ab]", "c",  true},
    {"[a-c]", "b",  true},
    {"[a-c]", "d", false},

    /* Corner cases in character class parsing */
    {"[a-c-e-g]",     "-",  true},
    {"[a-c-e-g]",     "d", false},
    {"[a-c-e-g]",     "f",  true},
    {"[]a-ceg-ik[]",  "a",  true},
    {"[]a-ceg-ik[]",  "]",  true},
    {"[]a-ceg-ik[]",  "[",  true},
    {"[]a-ceg-ik[]",  "h",  true},
    {"[]a-ceg-ik[]",  "f", false},
    {"[!]a-ceg-ik[]", "h", false},
    {"[!]a-ceg-ik[]", "]", false},
    {"[!]a-ceg-ik[]", "f",  true},

    /* Simple wild cards */
    {"?",   "a",    true},
    {"?",   "aa",  false},
    {"??",  "a",   false},
    {"?x?", "axb",  true},
    {"?x?", "abx", false},
    {"?x?", "xab", false},

    /* Asterisk wild cards (backtracking) */
    {"*??", "a",    false},
    {"*??", "ab",    true},
    {"*??", "abc",   true},
    {"*??", "abcd",  true},
    {"??*", "a",    false},
    {"??*", "ab",    true},
    {"??*", "abc",   true},
    {"??*", "abcd",  true},
    {"?*?", "a",    false},
    {"?*?", "ab",    true},
    {"?*?", "abc",   true},
    {"?*?", "abcd",  true},
    {"*b",  "b",     true},
    {"*b",  "ab",    true},
    {"*b",  "ba",   false},
    {"*b",  "bb",    true},
    {"*b",  "abb",   true},
    {"*b",  "bab",   true},
    {"*bc", "abbc",  true},
    {"*bc", "bc",    true},
    {"*bc", "bbc",   true},
    {"*bc", "bcbc",  true},

    /* Multiple asterisks (complex backtracking) */
    {"*ac*",              "abacadaeafag",         true},
    {"*ac*ae*ag*",        "abacadaeafag",         true},
    {"*a*b*[bc]*[ef]*g*", "abacadaeafag",         true},
    {"*a*b*[ef]*[cd]*g*", "abacadaeafag",        false},
    {"*abcd*",            "abcabcabcabcdefg",     true},
    {"*ab*cd*",           "abcabcabcabcdefg",     true},
    {"*abcd*abcdef*",     "abcabcdabcdeabcdefg",  true},
    {"*abcd*",            "abcabcabcabcefg",     false},
    {"*ab*cd*",           "abcabcabcabcefg",     false},
};

TESTSUITE(
    "glob:selftest", NULL, NULL,
    "glob selftest"
) {
    unsigned int count;

    for (count = 0; count < BFDEV_ARRAY_SIZE(test_glob); ++count) {
        const struct glob_test *test = test_glob + count;
        bool matched;

        matched = bfdev_glob(test->glob, test->string);
        bfdev_log_info("match '%s' with '%s': result %d\n",
                       test->string, test->glob, matched);

        if (matched != test->result) {
            bfdev_log_err("result should be %d\n", test->result);
            return -BFDEV_EFAULT;
        }
    }

    return -BFDEV_ENOERR;
}
