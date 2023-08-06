/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CTYPE_H_
#define _BFDEV_CTYPE_H_

#include <bfdev/config.h>

#if defined(__FreeBSD__) && defined(_KERNEL)
# include <sys/ctype.h>
#else
# include <ctype.h>
#endif

BFDEV_BEGIN_DECLS

enum bfdev_ctype_num {
    __BFDEV_CTYPE_UPPER = 0,
    __BFDEV_CTYPE_LOWER,
    __BFDEV_CTYPE_CNTRL,
    __BFDEV_CTYPE_PUNCT,
    __BFDEV_CTYPE_BLANK,
    __BFDEV_CTYPE_DIGIT,
    __BFDEV_CTYPE_HEXDG,
    __BFDEV_CTYPE_SPACE,
    __BFDEV_CTYPE_HDSPA,
    __BFDEV_CTYPE_ASCII,

    BFDEV_CTYPE_UPPER = (1UL << __BFDEV_CTYPE_UPPER),
    BFDEV_CTYPE_LOWER = (1UL << __BFDEV_CTYPE_LOWER),
    BFDEV_CTYPE_CNTRL = (1UL << __BFDEV_CTYPE_CNTRL),
    BFDEV_CTYPE_PUNCT = (1UL << __BFDEV_CTYPE_PUNCT),
    BFDEV_CTYPE_BLANK = (1UL << __BFDEV_CTYPE_BLANK),
    BFDEV_CTYPE_DIGIT = (1UL << __BFDEV_CTYPE_DIGIT),
    BFDEV_CTYPE_HEXDG = (1UL << __BFDEV_CTYPE_HEXDG),
    BFDEV_CTYPE_SPACE = (1UL << __BFDEV_CTYPE_SPACE),
    BFDEV_CTYPE_HDSPA = (1UL << __BFDEV_CTYPE_HDSPA),
    BFDEV_CTYPE_ASCII = (1UL << __BFDEV_CTYPE_ASCII),
};

extern const unsigned short
bfdev_ctype_table[256];

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isalpha_const(int c)
{
    return (((unsigned char)c | 32) - 'a') < 26;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isdigit_const(int c)
{
    return ((unsigned char)c - '0') < 10;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isblank_const(int c)
{
    return c == ' ' || c == '\t';
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_iscntrl_const(int c)
{
    return ((unsigned char)c < 0x20) || (c == 0x7f);
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isgraph_const(int c)
{
    return ((unsigned char)c - 0x21) < 0x5e;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_islower_const(int c)
{
    return ((unsigned char)c - 'a') < 26;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isprint_const(int c)
{
    return ((unsigned char)c - 0x20) < 0x5f;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isspace_const(int c)
{
    return (c == ' ') || ((unsigned char)c - '\t' < 5);
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isupper_const(int c)
{
    return ((unsigned char)c - 'A') < 26;
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isalnum_const(int c)
{
    return bfdev_isalpha_const(c) || bfdev_isdigit_const(c);
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_ispunct_const(int c)
{
    return bfdev_isgraph_const(c) && !bfdev_isalnum_const(c);
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_isxdigit_const(int c)
{
    return bfdev_isdigit_const(c) || (((unsigned char)c | 32) - 'a' < 6);
}

static __bfdev_attribute_const __bfdev_always_inline
int bfdev_ascii_const(int c)
{
    return (unsigned char)c <= 0x7f;
}

#define BFDEV_CTYPE_DYNAMIC(name, bitflags)     \
static inline __bfdev_attribute_const           \
int bfdev_##name##_dynamic(int c)               \
{                                               \
    return !!(bfdev_ctype_table[(int)           \
              (unsigned char)c] & (bitflags));  \
}

BFDEV_CTYPE_DYNAMIC(isalnum,
    BFDEV_CTYPE_UPPER | BFDEV_CTYPE_LOWER |
    BFDEV_CTYPE_DIGIT
)

BFDEV_CTYPE_DYNAMIC(isalpha,
    BFDEV_CTYPE_UPPER | BFDEV_CTYPE_LOWER
)

BFDEV_CTYPE_DYNAMIC(isblank,
    BFDEV_CTYPE_BLANK
)

BFDEV_CTYPE_DYNAMIC(iscntrl,
    BFDEV_CTYPE_CNTRL
)

BFDEV_CTYPE_DYNAMIC(isdigit,
    BFDEV_CTYPE_DIGIT
)

BFDEV_CTYPE_DYNAMIC(isgraph,
    BFDEV_CTYPE_PUNCT | BFDEV_CTYPE_UPPER |
    BFDEV_CTYPE_LOWER | BFDEV_CTYPE_DIGIT
)

BFDEV_CTYPE_DYNAMIC(islower,
    BFDEV_CTYPE_LOWER
)

BFDEV_CTYPE_DYNAMIC(isprint,
    BFDEV_CTYPE_PUNCT | BFDEV_CTYPE_UPPER |
    BFDEV_CTYPE_LOWER | BFDEV_CTYPE_DIGIT |
    BFDEV_CTYPE_HDSPA
)

BFDEV_CTYPE_DYNAMIC(ispunct,
    BFDEV_CTYPE_PUNCT
)

BFDEV_CTYPE_DYNAMIC(isspace,
    BFDEV_CTYPE_SPACE
)

BFDEV_CTYPE_DYNAMIC(isupper,
    BFDEV_CTYPE_UPPER
)

BFDEV_CTYPE_DYNAMIC(isxdigit,
    BFDEV_CTYPE_DIGIT | BFDEV_CTYPE_HEXDG
)

BFDEV_CTYPE_DYNAMIC(isascii,
    BFDEV_CTYPE_ASCII
)

#define bfdev_isalnum(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isalnum_const(__c)      \
    : bfdev_isalnum_dynamic(__c);   \
})

#define bfdev_isalpha(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isalpha_const(__c)      \
    : bfdev_isalpha_dynamic(__c);   \
})

#define bfdev_isblank(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isblank_const(__c)      \
    : bfdev_isblank_dynamic(__c);   \
})

#define bfdev_iscntrl(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_iscntrl_const(__c)      \
    : bfdev_iscntrl_dynamic(__c);   \
})

#define bfdev_isdigit(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isdigit_const(__c)      \
    : bfdev_isdigit_dynamic(__c);   \
})

#define bfdev_isgraph(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isgraph_const(__c)      \
    : bfdev_isgraph_dynamic(__c);   \
})

#define bfdev_islower(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_islower_const(__c)      \
    : bfdev_islower_dynamic(__c);   \
})

#define bfdev_isprint(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isprint_const(__c)      \
    : bfdev_isprint_dynamic(__c);   \
})

#define bfdev_ispunct(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_ispunct_const(__c)      \
    : bfdev_ispunct_dynamic(__c);   \
})

#define bfdev_isspace(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isspace_const(__c)      \
    : bfdev_isspace_dynamic(__c);   \
})

#define bfdev_isupper(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isupper_const(__c)      \
    : bfdev_isupper_dynamic(__c);   \
})

#define bfdev_isxdigit(c) ({        \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isxdigit_const(__c)     \
    : bfdev_isxdigit_dynamic(__c);  \
})

#define bfdev_isascii(c) ({         \
    int __c = (int)(c);             \
    __builtin_constant_p(__c)       \
    ? bfdev_isascii_const(__c)      \
    : bfdev_isascii_dynamic(__c);   \
})

static inline int
bfdev_toascii(int c)
{
    return (unsigned char)c & 0x7f;
}

static inline int
bfdev_tolower(int c)
{
    if (isupper(c))
        return (unsigned char)c | 32;
    return (unsigned char)c;
}

static inline int
bfdev_toupper(int c)
{
    if (islower(c))
        return (unsigned char)c & 0x5f;
    return (unsigned char)c;
}

BFDEV_END_DECLS

#endif /* _BFDEV_CTYPE_H_ */
