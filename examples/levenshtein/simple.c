/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/levenshtein.h>

int main(void)
{
    unsigned int diff;

    /* substitution distance reward */
    diff = bfdev_levenshtein(NULL, "aaa", "bbb", 1, 0, 1, 1);
    printf("levenshtein substitution: %u (aaa -> bbb)\n", diff);

    /* swap distance reward */
    diff = bfdev_levenshtein(NULL, "abcdef", "badcfe", 1, 1, 1, 1);
    printf("levenshtein swap: %u (abcdef -> badcfe)\n", diff);

    /* addition distance reward */
    diff = bfdev_levenshtein(NULL, "hello", "helloworld", 0, 0, 1, 0);
    printf("levenshtein addition: %u (hello -> helloworld)\n", diff);

    /* deletion distance reward */
    diff = bfdev_levenshtein(NULL, "popcat", "pop", 0, 0, 0, 1);
    printf("levenshtein deletion: %u (popcat -> pop)\n", diff);

    return 0;
}
