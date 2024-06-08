/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

/**
 * Array:
 *
 * The Array container provides a simple dynamically resizing
 * array, with separate tracking of capacity and usage. This
 * is typically useful when buffering I/O or processing data.
 */

#ifndef BFDEV_ARRAY_MSIZE
# define BFDEV_ARRAY_MSIZE 32
#endif

typedef struct bfdev_array bfdev_array_t;

struct bfdev_array {
    const bfdev_alloc_t *alloc;
    unsigned long capacity;
    unsigned long index;
    size_t cells;
    void *data;
};

#define BFDEV_ARRAY_STATIC(ALLOC, CELLS) { \
    .alloc = (ALLOC), .cells = (CELLS), \
}

#define BFDEV_ARRAY_INIT(alloc, cells) \
    (bfdev_array_t) BFDEV_ARRAY_STATIC(alloc, cells)

#define BFDEV_DEFINE_ARRAY(name, alloc, cells) \
    bfdev_array_t name = BFDEV_ARRAY_INIT(alloc, cells)

/**
 * bfdev_array_init() - initialize array.
 * @array: the array object.
 * @alloc: allocator used to allocate memory.
 * @cells: the size per elements.
 */
static inline void
bfdev_array_init(bfdev_array_t *array, const bfdev_alloc_t *alloc, size_t cells)
{
    *array = BFDEV_ARRAY_INIT(alloc, cells);
}

/**
 * bfdev_array_reset() - reset array.
 * @array: the array object.
 *
 * Reset the length of the stored data to zero and also
 * free the internal memory buffer
 */
static inline void
bfdev_array_reset(bfdev_array_t *array)
{
    array->index = 0;
}

/**
 * bfdev_array_index() - get index in array.
 * @array: the array object.
 *
 * Returns the number of elements stored in
 * the array container.
 */
static inline unsigned long
bfdev_array_index(const bfdev_array_t *array)
{
    return array->index;
}

/**
 * bfdev_array_size() - get total size in array.
 * @array: the array object.
 *
 * Returns the total size of elements stored in
 * the array container.
 */
static inline size_t
bfdev_array_size(const bfdev_array_t *array)
{
    return array->cells * array->index;
}

/**
 * bfdev_array_offset() - get elements offset in array.
 * @array: the array object.
 * @index: elements index.
 *
 * Return the address offset of the object indexed
 * by @index in the array.
 */
static inline uintptr_t
bfdev_array_offset(const bfdev_array_t *array, unsigned long index)
{
    return array->cells * index;
}

/**
 * bfdev_array_data() - get elements pointer in array.
 * @array: the array object.
 * @index: elements index.
 *
 * Return the offset value of the object indexed
 * by @index in the array.
 */
static inline void *
bfdev_array_data(const bfdev_array_t *array, unsigned long index)
{
    if (bfdev_unlikely(index >= array->index))
        return NULL;

    return array->data + bfdev_array_offset(array, index);
}

/**
 * bfdev_array_push() - push elements into the array.
 * @array: the array object.
 * @num: the data length to push.
 *
 * Creates a number of new elements on the array and
 * returns a pointer to the first of these elements.
 *
 * this may cause a re-allocation of the array depending on
 * the current state of the pool which in-turn could
 * cause a small performance hit.
 */
extern void *
bfdev_array_push(bfdev_array_t *array, unsigned long num);

extern void *
bfdev_array_pop(bfdev_array_t *array, unsigned long num);

extern void *
bfdev_array_peek(const bfdev_array_t *array, unsigned long num);

/**
 * bfdev_array_resize() - directly set the number of elements in array.
 * @array: the array object.
 * @num: the number required resize.
 */
extern int
bfdev_array_resize(bfdev_array_t *array, unsigned long num);

/**
 * bfdev_array_reserve() - reserve array buffer.
 * @array: the array object.
 * @num: the minimum required free space.
 *
 * Ensure that the buffer has space allocated for at least
 * @num bytes. If the current buffer is too small, it will
 * be reallocated, possibly to a larger size than requested.
 */
extern int
bfdev_array_reserve(bfdev_array_t *array, unsigned long num);

/**
 * bfdev_array_release() - release array.
 * @array: the array object
 *
 * Reset the length of the stored data to zero and also
 * free the internal memory buffer.
 */
extern void
bfdev_array_release(bfdev_array_t *array);

BFDEV_END_DECLS

#endif /* _BFDEV_ARRAY_H_ */
