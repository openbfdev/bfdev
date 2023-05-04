/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <bfdev/attributes.h>

#define EXPORT_SYMBOL(symbol) \
    extern typeof(symbol) symbol \
    __visibility("default")

#endif /* _EXPORT_H_ */
