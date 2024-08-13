/* SPDX-License-Identifier: LGPL-3.0-or-later */
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

/**
 * Textsearch:
 *
 * The textsearch infrastructure provides text searching facilities for
 * both linear and non-linear data. Individual search algorithms are
 * implemented in modules and chosen by the user.
 */

typedef struct bfdev_ts_state bfdev_ts_state_t;
typedef struct bfdev_ts_linear bfdev_ts_linear_t;
typedef struct bfdev_ts_context bfdev_ts_context_t;
typedef struct bfdev_ts_algorithm bfdev_ts_algorithm_t;

enum bfdev_ts_flags {
    __BFDEV_TS_IGCASE = 0,

    /* Searches string case insensitively */
    BFDEV_TS_IGCASE = BFDEV_BIT(__BFDEV_TS_IGCASE),
};

/**
 * struct bfdev_ts_state - search state.
 * @offset: offset for next match.
 * @pdata: search context private data.
 */
struct bfdev_ts_state {
    unsigned int offset;
    void *pdata;
};

/**
 * struct bfdev_ts_state - search linear state.
 * @offset: offset for next match.
 * @pdata: search context private data.
 */
struct bfdev_ts_linear {
    bfdev_ts_state_t tss;
    const char *data;
    unsigned int len;
};

/**
 * struct bfdev_ts_context - search context.
 * @flags: user flags.
 * @algo: chosen search algorithm.
 */
struct bfdev_ts_context {
    const bfdev_alloc_t *alloc;
    bfdev_ts_algorithm_t *algo;
    unsigned long flags;

    /**
     * next_block() - fetch next block of data.
	 * @tsc: search configuration.
	 * @tss: search state.
	 * @consumed: number of bytes consumed by the caller.
	 * @dest: destination buffer.
     *
	 * Called repeatedly until 0 is returned. Must assign the
	 * head of the next block of data to &*dst and return the length
	 * of the block or 0 if at the end.
     */
    unsigned int
    (*next_block)(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss,
                  unsigned int consumed, const void **dest);
};

/**
 * struct bfdev_ts_algorithm - search algorithm operations.
 * @name: name of search algorithm.
 */
struct bfdev_ts_algorithm {
    bfdev_list_head_t list;
    const char *name;

    /**
     * prepare() - initialization function to prepare a search.
     * @alloc: allocator used to allocate memory.
     * @pattern: pattern data
     * @len: length of pattern
     * @flags: search flags
     */
    bfdev_ts_context_t *
    (*prepare)(const bfdev_alloc_t *alloc, const void *pattern,
               size_t len, unsigned long flags);

    /**
     * destroy() - destroy algorithm specific parts of a search configuration.
	 * @tsc: search configuration.
     */
    void
    (*destroy)(bfdev_ts_context_t *tsc);

    /**
     * find() - find the next occurrence of the pattern.
	 * @tsc: search configuration.
     */
    unsigned int
    (*find)(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss);

    /**
     * pattern_get() - return pointer of pattern.
	 * @tsc: search configuration.
     */
    const void *
    (*pattern_get)(bfdev_ts_context_t *tsc);

    /**
     * pattern_len() - return length of pattern.
	 * @tsc: search configuration.
     */
    unsigned int
    (*pattern_len)(bfdev_ts_context_t *tsc);
};

BFDEV_BITFLAGS_STRUCT(
    bfdev_ts_igcase,
    bfdev_ts_context_t, flags,
    __BFDEV_TS_IGCASE
)

/**
 * bfdev_textsearch_find() - start searching for a pattern.
 * @tsc: search configuration.
 * @tss: search state.
 *
 * Returns the position of first occurrence of the pattern or
 * UINT_MAX if no match was found.
 */
static inline unsigned int
bfdev_textsearch_find(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;
    tss->offset = 0;

    return algo->find(tsc, tss);
}

/**
 * bfdev_textsearch_next() - continue searching for a pattern.
 * @tsc: search configuration.
 * @tss: search state.
 *
 * Continues a search looking for more occurrences of the pattern.
 * bfdev_textsearch_find() must be called to find the first occurrence
 * in order to reset the state.
 *
 * Returns the position of the next occurrence of the pattern or
 * UINT_MAX if not match was found.
 */
static inline unsigned int
bfdev_textsearch_next(bfdev_ts_context_t *tsc, bfdev_ts_state_t *tss)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;

    return algo->find(tsc, tss);
}

/**
 * textsearch_pattern_get() - return head of the pattern.
 * @tsc: search configuration.
 */
static inline const void *
textsearch_pattern_get(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;

    return algo->pattern_get(tsc);
}

/**
 * bfdev_textsearch_pattern_len() - return length of the pattern.
 * @tsc: search configuration.
 */
static inline unsigned int
bfdev_textsearch_pattern_len(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;

    return algo->pattern_len(tsc);
}

/**
 * bfdev_textsearch_destroy() - destroy a search configuration.
 * @tsc: search configuration.
 *
 * Releases all references of the configuration and frees
 * up the memory.
 */
static inline void
bfdev_textsearch_destroy(bfdev_ts_context_t *tsc)
{
    bfdev_ts_algorithm_t *algo;

    algo = tsc->algo;

    algo->destroy(tsc);
}

/**
 * bfdev_textsearch_linear() - search a pattern in linear data.
 * @tsc: search configuration.
 * @state: search state.
 * @data: data to search in.
 * @len: length of data.
 *
 * A simplified version of bfdev_textsearch_find() for continuous/linear data.
 * Call bfdev_textsearch_next() to retrieve subsequent matches.
 *
 * Returns the position of first occurrence of the pattern or
 * %UINT_MAX if no occurrence was found.
 */
extern unsigned int
bfdev_textsearch_linear(bfdev_ts_context_t *tsc, bfdev_ts_linear_t *linear,
                        const void *data, unsigned int len);

/**
 * bfdev_textsearch_create() - prepare a search.
 * @alloc: allocator used to allocate memory.
 * @name: name of search algorithm.
 * @pattern: pattern data.
 * @len: length of pattern.
 * @flags: search flags.
 */
extern bfdev_ts_context_t *
bfdev_textsearch_create(const bfdev_alloc_t *alloc, const char *name,
                        const void *pattern, size_t len, unsigned long flags);

/**
 * bfdev_textsearch_register() - register a textsearch module.
 * @algo: operations lookup table.
 *
 * Returns 0 or -BFDEV_EEXISTS if another module has already
 * registered with same name.
 */
extern int
bfdev_textsearch_register(bfdev_ts_algorithm_t *algo);

/**
 * bfdev_textsearch_unregister() - unregister a textsearch module.
 * @algo: operations lookup table.
 *
 * Returns 0 on success or -BFDEV_ENOENT if no matching
 * textsearch registration was found.
 */
extern int
bfdev_textsearch_unregister(bfdev_ts_algorithm_t *algo);

BFDEV_END_DECLS

#endif /* _BFDEV_TEXTSEARCH_H_ */
