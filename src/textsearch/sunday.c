/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/textsearch.h>

struct sunday_context {
    bfdev_ts_context_t tsc;
    unsigned int pattern_len;
    unsigned int shift_table[UINT8_MAX];
    uint8_t pattern[0];
};

#define ts_to_sunday(ptr) \
    bfdev_container_of(ptr, struct sunday_context, tsc)

static const void *
sunday_pattern_get(bfdev_ts_context_t *tsc)
{
    struct sunday_context *sctx = ts_to_sunday(tsc);
    return sctx->pattern;
}

static unsigned int
sunday_pattern_len(bfdev_ts_context_t *tsc)
{
    struct sunday_context *sctx = ts_to_sunday(tsc);
    return sctx->pattern_len;
}

static unsigned int
sunday_find(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss)
{
    #define find_pattern() (icase ? toupper(text[shift + index]) : text[shift + index])
    bool icase = bfdev_ts_test_igcase(tsc);
    struct sunday_context *sctx = ts_to_sunday(tsc);
    unsigned int consumed = tss->offset;
    unsigned int length, index, shift = 0;
    const uint8_t *text;

    for (;;) {
        length = tsc->next_block(tsc, tss, consumed, (const void **)&text);
        if (bfdev_unlikely(!length))
            return UINT_MAX;

        while (sctx->pattern_len + shift <= length) {
            for (index = 0; index < sctx->pattern_len; ++index) {
                if (sctx->pattern[index] != find_pattern())
                    break;
            }

            if (bfdev_unlikely(index == sctx->pattern_len))
                return consumed + shift;

            if (sctx->pattern_len >= length)
                break;

            shift += sctx->shift_table[text[shift + sctx->pattern_len]];
        }

        consumed += length;
    }

    #undef find_pattern
}

static inline void
sunday_compute_prefix(struct sunday_context *sctx, unsigned int flags)
{
    unsigned int index;

    for (index = 0; index < UINT8_MAX; ++index)
        sctx->shift_table[index] = sctx->pattern_len + 1;

    for (index = 0; index < sctx->pattern_len; ++index) {
        sctx->shift_table[sctx->pattern[index]] = sctx->pattern_len - index;
        if (flags & BFDEV_TS_IGCASE) {
            sctx->shift_table[tolower(sctx->pattern[index])] =
                sctx->pattern_len - index;
        }
    }
}

static bfdev_ts_context_t *
sunday_prepare(const struct bfdev_alloc *alloc, const void *pattern,
               size_t len, unsigned long flags)
{
    struct sunday_context *sctx;
    unsigned int index;

    sctx = bfdev_malloc(alloc, sizeof(*sctx) + len);
    if (!sctx)
        return NULL;

    sctx->tsc.flags = flags;
    sctx->pattern_len = len;

    if (!(flags & BFDEV_TS_IGCASE))
        memcpy(sctx->pattern, pattern, len);
    else for (index = 0; index < len; ++index)
        sctx->pattern[index] = toupper(((char *)pattern)[index]);
    sunday_compute_prefix(sctx, flags);

    return &sctx->tsc;
}

static void
sunday_destroy(bfdev_ts_context_t *tsc)
{
    struct sunday_context *sctx = ts_to_sunday(tsc);
    bfdev_free(tsc->alloc, sctx);
}

static bfdev_ts_algorithm_t
sunday_algorithm = {
    .name = "sunday",
    .find = sunday_find,
    .prepare = sunday_prepare,
    .destroy = sunday_destroy,
    .pattern_get = sunday_pattern_get,
    .pattern_len = sunday_pattern_len,
};

static __bfdev_ctor int
sunday_init(void)
{
    return bfdev_textsearch_register(&sunday_algorithm);
}
