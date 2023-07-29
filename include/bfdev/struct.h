/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_STRUCT_H_
#define _BFDEV_STRUCT_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/**
 * bfdev_struct_field - Report the size of a struct field in bytes.
 * @TYPE: The structure containing the field of interest
 * @MEMBER: The field to return the size of
 */
#define bfdev_struct_field(TYPE, MEMBER) \
    sizeof((((TYPE *)0)->MEMBER))

/**
 * __bfdev_struct_group - Create a mirrored named and anonyomous struct.
 * @TAG: The tag name for the named sub-struct (usually empty).
 * @NAME: The identifier name of the mirrored sub-struct.
 * @ATTRS: Any struct attributes (usually empty).
 * @MEMBERS: The member declarations for the mirrored structs.
 */
#define __bfdev_struct_group(TAG, NAME, ATTRS, MEMBERS...) \
    union { \
        struct { MEMBERS } ATTRS; \
        struct TAG { MEMBERS } ATTRS NAME; \
    }

/**
 * bfdev_struct_group - Wrap a set of declarations in a mirrored struct.
 * @name: The identifier name of the mirrored sub-struct.
 * @members: The member declarations for the mirrored structs.
 */
#define bfdev_struct_group(NAME, MEMBERS...)	\
    __bfdev_struct_group(/* no tag */, NAME, /* no attrs */, MEMBERS)

/**
 * bfdev_struct_group_attr - Create a struct group with trailing attributes.
 * @NAME: The identifier name of the mirrored sub-struct.
 * @ATTRS: Any struct attributes to apply.
 * @MEMBERS: The member declarations for the mirrored structs.
 */
#define bfdev_struct_group_attr(NAME, ATTRS, MEMBERS...) \
    __bfdev_struct_group(/* no tag */, NAME, ATTRS, MEMBERS)

BFDEV_END_DECLS

#endif /* _BFDEV_STRUCT_H_ */
