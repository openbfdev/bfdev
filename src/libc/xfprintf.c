/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/xprintf.h>
#include <bfdev/bits.h>
#include <export.h>

enum vxprintf_flags {
    __FL_ZERO = 0,  /* Zero modifier        */
    __FL_MINUS,     /* Minus modifier       */
    __FL_PLUS,      /* Plus modifier        */
    __FL_TICK,      /* ' modifier           */
    __FL_SPACE,     /* Space modifier       */
    __FL_HASH,      /* # modifier           */
    __FL_SIGNED,    /* Number is signed     */
    __FL_UPPER,     /* Upper case digits    */

    FL_ZERO     = BFDEV_BIT(__FL_ZERO),
    FL_MINUS    = BFDEV_BIT(__FL_MINUS),
    FL_PLUS     = BFDEV_BIT(__FL_PLUS),
    FL_TICK     = BFDEV_BIT(__FL_TICK),
    FL_SPACE    = BFDEV_BIT(__FL_SPACE),
    FL_HASH     = BFDEV_BIT(__FL_HASH),
    FL_SIGNED   = BFDEV_BIT(__FL_SIGNED),
    FL_UPPER    = BFDEV_BIT(__FL_UPPER),
};

enum vxprintf_state {
    STA_BARE = 0,
    STA_LPRE,
    STA_LLPRE,
    STA_HPRE,
    STA_HHPRE,
    STA_BIGLPRE,
    STA_ZTPRE,
    STA_JPRE,
    STA_STOP,

    STA_CHAR,
    STA_SHORT,
    STA_INT,
    STA_LONG,
    STA_LLONG,
    STA_UCHAR,
    STA_USHORT,
    STA_UINT,
    STA_ULONG,
    STA_ULLONG,
    STA_IMAX,
    STA_UMAX,
    STA_SIZET,
    STA_PTR,
    STA_PDIFF,
    STA_UIPTR,
    STA_MAXSTATE,
};

#define MAP(ch) ((ch) - 'A')
#define OOB(ch) ((unsigned)(ch) - 'A' > 'z' - 'A')

static const enum vxprintf_state
vxprintf_states[STA_STOP]['z' - 'A' + 1] = {
    [STA_BARE] = { /* 0: bare types */
        [MAP('l')] = STA_LPRE,
        [MAP('h')] = STA_HPRE,
        [MAP('L')] = STA_BIGLPRE,
        [MAP('z')] = STA_ZTPRE,
        [MAP('t')] = STA_ZTPRE,
        [MAP('j')] = STA_JPRE,
        [MAP('c')] = STA_CHAR,
        [MAP('d')] = STA_INT,
        [MAP('i')] = STA_INT,
        [MAP('C')] = STA_INT,
        [MAP('o')] = STA_UINT,
        [MAP('u')] = STA_UINT,
        [MAP('x')] = STA_UINT,
        [MAP('X')] = STA_UINT,
        [MAP('s')] = STA_PTR,
        [MAP('S')] = STA_PTR,
        [MAP('p')] = STA_UIPTR,
        [MAP('n')] = STA_PTR,
    },

    [STA_LPRE] = { /* 1: l-prefixed */
        [MAP('l')] = STA_LLPRE,
        [MAP('c')] = STA_INT,
        [MAP('d')] = STA_LONG,
        [MAP('i')] = STA_LONG,
        [MAP('o')] = STA_ULONG,
        [MAP('u')] = STA_ULONG,
        [MAP('x')] = STA_ULONG,
        [MAP('X')] = STA_ULONG,
        [MAP('s')] = STA_PTR,
        [MAP('n')] = STA_PTR,
    },

    [STA_LLPRE] = { /* 2: ll-prefixed */
        [MAP('d')] = STA_LLONG,
        [MAP('i')] = STA_LLONG,
        [MAP('o')] = STA_ULLONG,
        [MAP('u')] = STA_ULLONG,
        [MAP('x')] = STA_ULLONG,
        [MAP('X')] = STA_ULLONG,
        [MAP('n')] = STA_PTR,
    },

    [STA_HPRE] = { /* 3: h-prefixed */
        [MAP('h')] = STA_HHPRE,
        [MAP('d')] = STA_SHORT,
        [MAP('i')] = STA_SHORT,
        [MAP('o')] = STA_USHORT,
        [MAP('u')] = STA_USHORT,
        [MAP('x')] = STA_USHORT,
        [MAP('X')] = STA_USHORT,
        [MAP('n')] = STA_PTR,
    },

    [STA_HHPRE] = { /* 4: hh-prefixed */
        [MAP('d')] = STA_CHAR,
        [MAP('i')] = STA_CHAR,
        [MAP('o')] = STA_UCHAR,
        [MAP('u')] = STA_UCHAR,
        [MAP('x')] = STA_UCHAR,
        [MAP('X')] = STA_UCHAR,
        [MAP('n')] = STA_PTR,
    },

    [STA_BIGLPRE] = { /* 5: L-prefixed */
        [MAP('n')] = STA_PTR,
    },

    [STA_ZTPRE] = { /* 6: z- or t-prefixed (assumed to be same size) */
        [MAP('o')] = STA_SIZET,
        [MAP('u')] = STA_SIZET,
        [MAP('x')] = STA_SIZET,
        [MAP('X')] = STA_SIZET,
        [MAP('d')] = STA_PDIFF,
        [MAP('i')] = STA_PDIFF,
        [MAP('n')] = STA_PTR,
    },

    [STA_JPRE] = { /* 7: j-prefixed */
        [MAP('d')] = STA_IMAX,
        [MAP('i')] = STA_IMAX,
        [MAP('o')] = STA_UMAX,
        [MAP('u')] = STA_UMAX,
        [MAP('x')] = STA_UMAX,
        [MAP('X')] = STA_UMAX,
        [MAP('n')] = STA_PTR,
    }
};

union vxprintf_arg {
    bfdev_uintmax_t integer;
    void *pointer;
};

static const char
vxprintf_xdigits[16] = {
    "0123456789ABCDEF"
};

static char *
format_hex(bfdev_uintmax_t x, char *s, int lower)
{
    for (; x; x >>= 4)
        *--s = vxprintf_xdigits[x & 15] | lower;

    return s;
}

static char *
format_oct(bfdev_uintmax_t x, char *s)
{
    for (; x; x >>= 3)
        *--s = '0' + (x & 7);

    return s;
}

static char *
format_dec(bfdev_uintmax_t x, char *s)
{
    unsigned long y;

    for (; x > BFDEV_ULONG_MAX; x /= 10)
        *--s = '0' + x % 10;

    for (y = x; y; y /= 10)
        *--s = '0' + y % 10;

    return s;
}

static bfdev_bool
vxprintf_arg(union vxprintf_arg *arg, enum vxprintf_state state, bfdev_va_list *ap)
{
#define VXPRINTF_VAARG(value, type, force, size) case type: { \
       arg->value = (force)bfdev_va_arg(*ap, size); \
       break; \
    }

    switch (state) {
        VXPRINTF_VAARG(integer, STA_CHAR, signed char, int)
        VXPRINTF_VAARG(integer, STA_SHORT, signed short, int)
        VXPRINTF_VAARG(integer, STA_INT, signed int, int)
        VXPRINTF_VAARG(integer, STA_LONG, signed long, long)
        VXPRINTF_VAARG(integer, STA_LLONG, signed long long, long long)
        VXPRINTF_VAARG(integer, STA_UCHAR, unsigned char, int)
        VXPRINTF_VAARG(integer, STA_USHORT, unsigned short, int)
        VXPRINTF_VAARG(integer, STA_UINT, unsigned int, unsigned int)
        VXPRINTF_VAARG(integer, STA_ULONG, unsigned long, unsigned long)
        VXPRINTF_VAARG(integer, STA_ULLONG, unsigned long long, unsigned long long)
        VXPRINTF_VAARG(integer, STA_IMAX, bfdev_intmax_t, bfdev_intmax_t)
        VXPRINTF_VAARG(integer, STA_UMAX, bfdev_uintmax_t, bfdev_uintmax_t)
        VXPRINTF_VAARG(integer, STA_SIZET, bfdev_size_t, bfdev_size_t)
        VXPRINTF_VAARG(integer, STA_PDIFF, bfdev_ptrdiff_t, bfdev_ptrdiff_t)
        VXPRINTF_VAARG(integer, STA_UIPTR, bfdev_uintptr_t, void *)
        VXPRINTF_VAARG(pointer, STA_PTR, void *, void *)
        default: return bfdev_false;
    }

    return bfdev_true;
#undef VXPRINTF_VAARG
}

static int
vxprintf_getint(const char **str)
{
    int value;

    for (value = 0; bfdev_isdigit(**str); ++*str) {
        if (value <= BFDEV_INT_MAX / 10 && **str - '0' <= BFDEV_INT_MAX - 10 * value)
            value = 10 * value + (**str - '0');
        else
            value = -1;
    }

    return value;
}

static void
vxprintf_write(bfdev_vxprintf_t *ctx, const char *str, bfdev_size_t len)
{
    if (len && ctx->write)
        ctx->write(str, len, ctx->pdata);
}

static void
vxprintf_pad(bfdev_vxprintf_t *ctx, char ch, int width, int len, int flags)
{
    char buff[256];

    if (flags & (FL_MINUS | FL_ZERO) || len >= width)
        return;

    len = width - len;
    bfdev_memset(buff, ch, bfdev_min(len, sizeof(buff)));

    for (; len >= sizeof(buff); len -= sizeof(buff))
        vxprintf_write(ctx, buff, sizeof(buff));
    vxprintf_write(ctx, buff, len);
}

static bfdev_size_t
vxprintf_core(bfdev_vxprintf_t *ctx, const char *fmt, bfdev_va_list *args)
{
    enum vxprintf_state state, prev;
    union vxprintf_arg arg;
    const char *prefix;
    char *start, *end;
    char buff[64], ch;
    int flags, width, prec;
    bfdev_size_t slen, count, plen;
    bfdev_bool xp;

    for (count = 0; *fmt; count += slen) {
        /* Handle literal text */
        start = (char *)fmt;
        while (*fmt && *fmt != '%')
            fmt++;

        /* Handle %% format specifiers */
        for (end = (char *)fmt; fmt[0] == '%'; fmt += 2, ++end) {
            if (fmt[1] != '%')
                break;
        }

        slen = end - start;
        if (slen) {
            vxprintf_write(ctx, start, slen);
            continue;
        }

        /* Read modifier flags */
        for (flags = 0, fmt++; *fmt; ++fmt) {
            switch (*fmt) {
                case '#':
                    flags |= FL_HASH;
                    continue;

                case '+':
                    flags |= FL_PLUS;
                    continue;

                case '-':
                    flags |= FL_MINUS;
                    continue;

                case '0':
                    flags |= FL_ZERO;
                    continue;

                case ' ':
                    flags |= FL_SPACE;
                    continue;

                case '\'':
                    flags |= FL_TICK;
                    continue;

                default:
                    break;
            }
            break;
        }

        /* Read field width */
        if (fmt[0] == '*') {
            width = bfdev_va_arg(*args, int);
            if (width < 0) {
                width = -width;
                flags |= FL_MINUS;
            }
            fmt++;
        } else {
            width = vxprintf_getint(&fmt);
            if (bfdev_unlikely(width < 0))
                return BFDEV_SIZE_MAX;
        }

        /* Read precision */
        if (fmt[0] == '.') {
            if (fmt[1] == '*') {
                prec = bfdev_va_arg(*args, int);
                if (prec < 0)
                    prec = -1;
                xp = prec >= 0;
                fmt += 2;
            } else {
                fmt++;
                prec = vxprintf_getint(&fmt);
                if (bfdev_unlikely(prec < 0))
                    return BFDEV_SIZE_MAX;
                xp = bfdev_true;
            }
        } else {
            prec = -1;
            xp = bfdev_false;
        }

        /* Format specifier state machine */
        for (state = STA_BARE; state < STA_STOP; ++fmt) {
            if (bfdev_unlikely(OOB(*fmt)))
                return BFDEV_SIZE_MAX;

            prev = state;
            state = vxprintf_states[state][MAP(*fmt)];
            if (state == STA_BARE)
                break;
        }

        if (bfdev_unlikely(state == STA_BARE))
            return BFDEV_SIZE_MAX;

        if (!vxprintf_arg(&arg, state, args))
            return BFDEV_SIZE_MAX;

        end = buff + sizeof(buff);
        prefix = "";
        ch = fmt[-1];

        /* - and 0 flags are mutually exclusive */
        if (flags & FL_MINUS)
            flags &= ~FL_ZERO;

        switch (ch) {
            case 'n':
                switch(prev) {
                    case STA_BARE: default:
                        *(int *)arg.pointer = count;
                        break;

                    case STA_LPRE:
                        *(long *)arg.pointer = count;
                        break;

                    case STA_LLPRE:
                        *(long long *)arg.pointer = count;
                        break;

                    case STA_HPRE:
                        *(unsigned short *)arg.pointer = count;
                        break;

                    case STA_HHPRE:
                        *(unsigned char *)arg.pointer = count;
                        break;

                    case STA_ZTPRE:
                        *(bfdev_size_t *)arg.pointer = count;
                        break;

                    case STA_JPRE:
                        *(bfdev_uintmax_t *)arg.pointer = count;
                        break;
                }
                continue;

            case 'c': /* char */
                prec = 1;
                start = end - 1;
                *start = arg.integer;
                flags &= ~FL_ZERO;
                break;

            case 's': /* string */
                start = arg.pointer ?: "(null)";
                end = start + bfdev_strnlen(start, prec < 0 ? BFDEV_INT_MAX : prec);
                if (prec < 0 && *end)
                    return BFDEV_SIZE_MAX;
                prec = end - start;
                flags &= ~FL_ZERO;
                break;

            case 'p': /* pointer */
                prec = bfdev_max(prec, 2 * sizeof(void *));
                flags |= FL_HASH;
                ch = 'x';
                bfdev_fallthrough;

            case 'x': case 'X': /* hex */
                start = format_hex(arg.integer, end, ch & 32);
                if (flags & FL_HASH && arg.integer)
                    prefix = ch == 'x' ? "0x" : "0X";
                goto number;

            case 'o': /* oct */
                start = format_oct(arg.integer, end);
                if ((flags & FL_HASH) && prec < end - start + 1)
                    prec = end - start + 1;
                goto number;

            case 'd': case 'i': /* dec */
                if (arg.integer <= BFDEV_INTMAX_MAX) {
                    if (flags & FL_PLUS)
                        prefix = "+";
                    else if (flags & FL_SPACE)
                        prefix = " ";
                } else {
                    prefix = "-";
                    arg.integer = -arg.integer;
                }
                bfdev_fallthrough;

            case 'u':
                start = format_dec(arg.integer, end);
            number:
                if (xp) {
                    if (prec < 0)
                        return BFDEV_SIZE_MAX;
                    flags &= ~FL_ZERO;
                }
                if (!arg.integer && !prec) {
                    start = end;
                    break;
                }
                bfdev_max_adj(prec, end - start + !arg.integer);
                break;

            default:
                return BFDEV_SIZE_MAX;
        }

        plen = bfdev_strlen(prefix);
        bfdev_max_adj(prec, end - start);
        if (prec > BFDEV_INT_MAX - plen)
            return BFDEV_SIZE_MAX;

        bfdev_max_adj(width, plen + prec);
        if (width > BFDEV_INT_MAX - count)
            return BFDEV_SIZE_MAX;

        vxprintf_pad(ctx, ' ', width, plen + prec, flags);
        vxprintf_write(ctx, prefix, plen);

        vxprintf_pad(ctx, '0', width, plen + prec, flags ^ FL_ZERO);
        vxprintf_pad(ctx, '0', prec, end - start, 0);
        vxprintf_write(ctx, start, end - start);
        vxprintf_pad(ctx, ' ', width, plen + prec, flags ^ FL_MINUS);
        slen = width;
    }

    return count;
}

export bfdev_size_t
bfdev_vxfprintf(bfdev_vxprintf_t *ctx, const char *fmt, bfdev_va_list args)
{
    bfdev_va_list argbuf;
    bfdev_size_t len;

    bfdev_va_copy(argbuf, args);
    len = vxprintf_core(ctx, fmt, &argbuf);
    bfdev_va_end(argbuf);

    return len;
}

export bfdev_size_t
bfdev_xfprintf(bfdev_vxprintf_t *ctx, const char *fmt, ...)
{
    bfdev_va_list args;
    bfdev_size_t len;

    bfdev_va_start(args, fmt);
    len = bfdev_vxfprintf(ctx, fmt, args);
    bfdev_va_end(args);

    return len;
}
