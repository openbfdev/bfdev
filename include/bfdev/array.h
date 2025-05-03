/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ARRAY_H_
#define _BFDEV_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/types.h>
#include <bfdev/stddef.h>
#include <bfdev/string.h>
#include <bfdev/allocator.h>
#include <bfdev/guards.h>

BFDEV_BEGIN_DECLS

/**
 * Array:
 *
 * The Array container provides a simple dynamically resizing
 * array, with separate tracking of capacity and usage. This
 * is typically useful when buffering I/O or processing data.
 */

#ifndef BFDEV_ARRAY_MINSIZE
# define BFDEV_ARRAY_MINSIZE 32
#endif

typedef struct bfdev_array bfdev_array_t;

struct bfdev_array {
    const bfdev_alloc_t *alloc;
    unsigned long capacity;
    unsigned long index;
    unsigned long seek;
    bfdev_size_t cells;
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
bfdev_array_init(bfdev_array_t *array, const bfdev_alloc_t *alloc,
                 bfdev_size_t cells)
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
 * bfdev_array_tell() - get current position in array.
 * @array: the array object.
 *
 * Returns the current seek position of the array.
 */
static inline unsigned long
bfdev_array_tell(const bfdev_array_t *array)
{
    return array->seek;
}

/**
 * bfdev_array_offset() - get elements offset in array.
 * @array: the array object.
 * @index: elements index.
 *
 * Return the address offset of the object indexed
 * by @index in the array.
 */
static inline bfdev_uintptr_t
bfdev_array_offset(const bfdev_array_t *array, unsigned long index)
{
    return array->cells * index;
}

/**
 * bfdev_array_size() - get total size in array.
 * @array: the array object.
 *
 * Returns the total size of elements stored in
 * the array container.
 */
static inline bfdev_size_t
bfdev_array_size(const bfdev_array_t *array)
{
    return bfdev_array_offset(array, array->index);
}

/**
 * bfdev_array_remain() - get remain size in array.
 * @array: the array object.
 *
 * Returns the remain size of elements stored in
 * the array container.
 */
static inline bfdev_size_t
bfdev_array_remain(const bfdev_array_t *array)
{
    if (array->index <= array->seek)
        return 0;

    return bfdev_array_offset(array, array->index - array->seek);
}

/**
 * bfdev_array_seek() - seek elements in the array.
 * @array: the array object.
 * @seek: the number of element to seek.
 *
 * Set the current position of the array to @seek.
 * The next call to bfdev_array_data() will start
 * from this position.
 */
static inline void
bfdev_array_seek(bfdev_array_t *array, unsigned long seek)
{
    array->seek = seek;
}

/**
 * bfdev_array_push() - push elements into the array.
 * @array: the array object.
 * @num: the number of element to push.
 *
 * Creates a number of new elements at the end of array and
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
 * bfdev_array_data() - get elements pointer in array.
 * @array: the array object.
 * @index: elements index.
 *
 * Return the offset value of the object indexed
 * by @index in the array.
 */
extern void *
bfdev_array_data(const bfdev_array_t *array, unsigned long index);

/**
 * bfdev_array_read() - read elements from the array.
 * @array: the array object.
 * @num: the number of element to read.
 *
 * Retrieves a block of @num elements starting from the current seek position
 * in the array and returns a pointer to the first element.
 */
extern const void *
bfdev_array_read(bfdev_array_t *array, unsigned long num);

/**
 * bfdev_array_write() - write elements to the array.
 * @array: the array object.
 * @num: the number of element to write.
 *
 * Reserves space for @num elements starting from the current seek position,
 * allocating memory if necessary, and returns a pointer to the first element.
 * The caller is responsible for writing the data to the return memory.
 */
extern void *
bfdev_array_write(bfdev_array_t *array, unsigned long num);

/**
 * bfdev_array_append() - append elements into the array.
 * @array: the array object.
 * @data: the elements to append.
 * @num: the number of element to append.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_array_append(bfdev_array_t *array, const void *data, unsigned long num);

/**
 * bfdev_array_remove() - remove elements from the array.
 * @array: the array object.
 * @index: the index of the first elements to remove.
 * @num: the number of element to remove.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_array_remove(bfdev_array_t *array, unsigned long index, unsigned long num);

/**
 * bfdev_array_resize() - directly set the number of elements in array.
 * @array: the array object.
 * @num: the number required resize.
 *
 * Return 0 on success or a negative error code on failure.
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
 *
 * Return 0 on success or a negative error code on failure.
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

static inline bfdev_array_t *
bfdev_array_create(const bfdev_alloc_t *alloc, bfdev_size_t cells)
{
    bfdev_array_t *array;

    array = bfdev_malloc(alloc, sizeof(*array));
    if (bfdev_unlikely(!array))
        return BFDEV_NULL;
    bfdev_array_init(array, alloc, cells);

    return array;
}

static inline void
bfdev_array_destroy(bfdev_array_t *array)
{
    bfdev_array_release(array);
    bfdev_free(array->alloc, array);
}

BFDEV_DEFINE_CLASS(bfdev_array, bfdev_array_t *,
    bfdev_array_create(alloc, cells),
    bfdev_array_destroy(_T),
    const bfdev_alloc_t *alloc, bfdev_size_t cells
)

BFDEV_END_DECLS

#endif /* _BFDEV_ARRAY_H_ */
