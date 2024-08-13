/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BITFIELD_H_
#define _BFDEV_BITFIELD_H_

#include <bfdev/config.h>
#include <bfdev/bitops.h>

BFDEV_BEGIN_DECLS

/**
 * BFDEV_FIELD_GET() - extract a bitfield element.
 * @mask: shifted mask defining the field's length and position.
 * @reg: value of entire bitfield.
 */
#define BFDEV_FIELD_GET(mask, reg) ({ \
    typeof(mask) _mask = (mask); \
    (typeof(mask))(((reg) & (_mask)) >> bfdev_ffsuf(_mask)); \
})

/**
 * BFDEV_FIELD_PREP() - prepare a bitfield element.
 * @mask: shifted mask defining the field's length and position.
 * @val: value to put in the field.
 */
#define BFDEV_FIELD_PREP(mask, val) ({ \
    typeof(mask) _mask = (mask); \
    ((typeof(mask))(val) << bfdev_ffsuf(_mask)) & (_mask); \
})

/**
 * BFDEV_FIELD_FIT() - check if value fits in the field.
 * @mask: shifted mask defining the field's length and position.
 * @val: value to test against the field.
 */
#define BFDEV_FIELD_FIT(mask, val) ({ \
    typeof(mask) _mask = (mask); \
    !((((typeof(mask))(val)) << bfdev_ffsuf(_mask)) & ~(_mask)); \
})

/**
 * BFDEV_FIELD_MAX() - produce the maximum value representable by a field.
 * @mask: shifted mask defining the field's length and position.
 */
#define BFDEV_FIELD_MAX(mask) ({ \
    typeof(mask) _mask = (mask); \
    (typeof(mask))((_mask) >> bfdev_ffsuf(_mask)); \
})

BFDEV_END_DECLS

#endif /* _BFDEV_BITFIELD_H_ */
