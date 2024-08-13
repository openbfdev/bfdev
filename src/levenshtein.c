/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/levenshtein.h>
#include <export.h>

export unsigned int
bfdev_levenshtein_len(const bfdev_alloc_t *alloc,
                      const char *str1, const char *str2,
                      size_t len1, size_t len2,
                      unsigned int s, unsigned int w,
                      unsigned int a, unsigned int d)
{
    unsigned int *row1, *row2, *row3;
    unsigned int distance, *cache;
    size_t index1, index2;

    if (bfdev_unlikely(!len1))
        return len2 * a;

    if (bfdev_unlikely(!len2))
        return len1 * d;

    cache = bfdev_malloc(alloc, BFDEV_BYTES_PER_INT * (len1 + 1) * 3);
    if (bfdev_unlikely(!cache))
        return UINT_MAX;

    row1 = cache;
    row2 = row1 + (len1 + 1);
    row3 = row2 + (len1 + 1);

    for (index1 = 0; index1 < len1; ++index1)
        row2[index1] = index1 * d;

    for (index2 = 0; index2 < len2; ++index2) {
        row3[0] = (index2 + 1) * a;

        for (index1 = 0; index1 < len1; ++index1) {
            /* substitution distance reward */
            row3[index1 + 1] = row2[index1] + s * (str1[index1] != str2[index2]);

            /* swap distance reward */
            if (index2 && index1 && str1[index1] == str2[index2 - 1] &&
                str1[index1 - 1] == str2[index2] && row3[index1 + 1] > row1[index1 - 1] + w)
                row3[index1 + 1] = row1[index1 - 1] + w;

            /* addition distance reward */
            if (row3[index1 + 1] > row2[index1 + 1] + a)
                row3[index1 + 1] = row2[index1 + 1] + a;

            /* deletion distance reward */
            if (row3[index1 + 1] > row3[index1] + d)
                row3[index1 + 1] = row3[index1] + d;
        }

        bfdev_swap(row1, row2);
        bfdev_swap(row2, row3);
    }

    distance = row2[len1];
    bfdev_free(alloc, cache);

    return distance;
}

export unsigned int
bfdev_levenshtein(const bfdev_alloc_t *alloc,
                  const char *str1, const char *str2,
                  unsigned int s, unsigned int w,
                  unsigned int a, unsigned int d)
{
    size_t len1, len2;

    len1 = bfport_strlen(str1);
    len2 = bfport_strlen(str2);

    return bfdev_levenshtein_len(alloc, str1, str2, len1, len2, s, w, a, d);
}
