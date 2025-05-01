/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_TEMPLATE_ARRAY_H_
#define _BFDEV_TEMPLATE_ARRAY_H_

#include <bfdev/config.h>
#include <bfdev/array.h>
#include <bfdev/buff.h>

BFDEV_BEGIN_DECLS

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
 * bfdev_array_append_array() - append array into the array.
 * @array: the array object.
 * @append: the array to append.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_array_append_array(bfdev_array_t *array, const bfdev_array_t *append);

/**
 * bfdev_array_append_buff() - append buff into the array.
 * @array: the array object.
 * @append: the buff to append.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_array_append_buff(bfdev_array_t *array, bfdev_buff_t *append);

/**
 * bfdev_array_append_cstr() - append cstr into the array.
 * @array: the array object.
 * @append: the cstr to append.
 *
 * Return 0 on success or a negative error code on failure.
 */
extern int
bfdev_array_append_cstr(bfdev_array_t *array, const char *append);

BFDEV_END_DECLS

#endif /* _BFDEV_TEMPLATE_ARRAY_H_ */
