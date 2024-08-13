/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LEVENSHTEIN_H_
#define _BFDEV_LEVENSHTEIN_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_levenshtein_len - Calculation string levenshtein edit distance.
 * @str1: string1 to calculation distance.
 * @str2: string2 to calculation distance.
 * @len1: the length of @str1.
 * @len2: the length of @str2.
 * @s: substitution distance reward.
 * @w: swap distance reward.
 * @a: addition distance reward.
 * @d: deletion distance reward.
 */
extern unsigned int
bfdev_levenshtein_len(const bfdev_alloc_t *alloc,
                      const char *str1, const char *str2,
                      size_t len1, size_t len2,
                      unsigned int s, unsigned int w,
                      unsigned int a, unsigned int d);

/**
 * bfdev_levenshtein - Calculation string levenshtein edit distance.
 * @str1: string1 to calculation distance.
 * @str2: string2 to calculation distance.
 * @s: substitution distance reward.
 * @w: swap distance reward.
 * @a: addition distance reward.
 * @d: deletion distance reward.
 */
extern unsigned int
bfdev_levenshtein(const bfdev_alloc_t *alloc,
                  const char *str1, const char *str2,
                  unsigned int s, unsigned int w,
                  unsigned int a, unsigned int d);

BFDEV_END_DECLS

#endif /* _BFDEV_LEVENSHTEIN_H_ */
