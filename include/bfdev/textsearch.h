/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TEXTSEARCH_H_
#define _BFDEV_TEXTSEARCH_H_

#include <bfdev/config.h>
#include <bfdev/list.h>
#include <bfdev/bitflags.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

enum bfdev_ts_flags {
    __BFDEV_TS_IGCASE = 0,
    BFDEV_TS_IGCASE = BFDEV_BIT(__BFDEV_TS_IGCASE),
};

/**
 * struct bfdev_ts_state - Search state.
 * @offset: offset for next match.
 * @offset: search context private data.
 */
struct bfdev_ts_state {
    unsigned int offset;
    void *pdata;
};

struct bfdev_ts_linear {
    struct bfdev_ts_state tss;
    const char *data;
    unsigned int len;
};

/**
 * struct bfdev_ts_context - Search context.
 * @ops: chosen search algorithm.
 * @flags: user flags.
 * @next_block: fetch next block of data.
 */
struct bfdev_ts_context {
    const struct bfdev_alloc *alloc;
    unsigned long flags;

    struct bfdev_ts_algorithm *algo;
    unsigned int (*next_block)(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss,
                               unsigned int consumed, const void **dest);
};

/**
 * struct bfdev_ts_algorithm - Search algorithm operations.
 * @name: name of search algorithm.
 * @prepare: initialization function to prepare a search.
 * @destroy: destroy algorithm specific parts of a search configuration.
 * @find: find the next occurrence of the pattern.
 * @pattern_get: return pointer of pattern.
 * @pattern_len: return length of pattern.
 */
struct bfdev_ts_algorithm {
    struct bfdev_list_head list;
    const char *name;

    struct bfdev_ts_context *(*prepare)(const struct bfdev_alloc *alloc, const void *pattern,
                                        size_t len, unsigned long flags);
    void (*destroy)(struct bfdev_ts_context *tsc);
    unsigned int (*find)(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss);
    const void *(*pattern_get)(struct bfdev_ts_context *tsc);
    unsigned int (*pattern_len)(struct bfdev_ts_context *tsc);
};

BFDEV_BITFLAGS_STRUCT(bfdev_ts, struct bfdev_ts_context, flags)
BFDEV_BITFLAGS_STRUCT_FLAG(bfdev_ts, struct bfdev_ts_context, flags, igcase, __BFDEV_TS_IGCASE)

static inline unsigned int
bfdev_textsearch_find(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    tss->offset = 0;
    return algo->find(tsc, tss);
}

static inline unsigned int
bfdev_textsearch_next(struct bfdev_ts_context *tsc, struct bfdev_ts_state *tss)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    return algo->find(tsc, tss);
}

static inline const void *
textsearch_pattern_get(struct bfdev_ts_context *tsc)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    return algo->pattern_get(tsc);
}

static inline unsigned int
bfdev_textsearch_pattern_len(struct bfdev_ts_context *tsc)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    return algo->pattern_len(tsc);
}

static inline void
bfdev_textsearch_destroy(struct bfdev_ts_context *tsc)
{
    struct bfdev_ts_algorithm *algo = tsc->algo;
    algo->destroy(tsc);
}

extern unsigned int
bfdev_textsearch_linear_find(struct bfdev_ts_context *tsc, struct bfdev_ts_linear *linear,
                             const void *data, unsigned int len);

extern unsigned int
bfdev_textsearch_linear_next(struct bfdev_ts_context *tsc, struct bfdev_ts_linear *linear);

extern struct bfdev_ts_context *
bfdev_textsearch_create(const struct bfdev_alloc *alloc, const char *name,
                        const void *pattern, size_t len, unsigned long flags);

extern int
bfdev_textsearch_register(struct bfdev_ts_algorithm *algo);

extern void
bfdev_textsearch_unregister(struct bfdev_ts_algorithm *algo);

BFDEV_END_DECLS

#endif /* _BFDEV_TEXTSEARCH_H_ */
