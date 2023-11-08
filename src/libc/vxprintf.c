/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/stdio.h>
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
#define OOB(ch) (MAP(ch) > 'z' - 'A')

static const enum vxprintf_state
vxprintf_states[STA_STOP]['z' - 'A' + 1] = {
    { /* 0: bare types */
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

    { /* 1: l-prefixed */
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

    { /* 2: ll-prefixed */
		[MAP('d')] = STA_LLONG,
        [MAP('i')] = STA_LLONG,
		[MAP('o')] = STA_ULLONG,
        [MAP('u')] = STA_ULLONG,
		[MAP('x')] = STA_ULLONG,
        [MAP('X')] = STA_ULLONG,
		[MAP('n')] = STA_PTR,
	},

    { /* 3: h-prefixed */
		[MAP('h')] = STA_HHPRE,
		[MAP('d')] = STA_SHORT,
        [MAP('i')] = STA_SHORT,
		[MAP('o')] = STA_USHORT,
        [MAP('u')] = STA_USHORT,
		[MAP('x')] = STA_USHORT,
        [MAP('X')] = STA_USHORT,
		[MAP('n')] = STA_PTR,
	},

    { /* 4: hh-prefixed */
		[MAP('d')] = STA_CHAR,
        [MAP('i')] = STA_CHAR,
		[MAP('o')] = STA_UCHAR,
        [MAP('u')] = STA_UCHAR,
		[MAP('x')] = STA_UCHAR,
        [MAP('X')] = STA_UCHAR,
		[MAP('n')] = STA_PTR,
	},

    { /* 5: L-prefixed */
		[MAP('n')] = STA_PTR,
	},

    { /* 6: z- or t-prefixed (assumed to be same size) */
		[MAP('o')] = STA_SIZET,
        [MAP('u')] = STA_SIZET,
		[MAP('x')] = STA_SIZET,
        [MAP('X')] = STA_SIZET,
		[MAP('d')] = STA_PDIFF,
        [MAP('i')] = STA_PDIFF,
		[MAP('n')] = STA_PTR,
	},

    { /* 7: j-prefixed */
		[MAP('d')] = STA_IMAX,
        [MAP('i')] = STA_IMAX,
		[MAP('o')] = STA_UMAX,
        [MAP('u')] = STA_UMAX,
		[MAP('x')] = STA_UMAX,
        [MAP('X')] = STA_UMAX,
		[MAP('n')] = STA_PTR,
	}
};

struct context {

};

union vxprintf_arg {
    uintmax_t integer;
    void *pointer;
};

static char *
format_hex(uintmax_t x, char *s, int lower)
{
	for (; x; x >>= 4)
        *--s = xdigits[(x & 15)] | lower;
	return s;
}

static char *
format_oct(uintmax_t x, char *s)
{
	for (; x; x>>=3) *--s = '0' + (x&7);
	return s;
}

static char *
format_dec(uintmax_t x, char *s)
{
	unsigned long y;
	for (   ; x>ULONG_MAX; x/=10) *--s = '0' + x%10;
	for (y=x;           y; y/=10) *--s = '0' + y%10;
	return s;
}

static bool
vxprintf_arg(union vxprintf_arg *arg, enum vxprintf_state state, va_list *ap)
{
	switch (state) {
        default: return false;
	    break; case STA_CHAR:   arg->integer = (signed char)va_arg(*ap, int);
	    break; case STA_SHORT:  arg->integer = (short)va_arg(*ap, int);
	    break; case STA_INT:    arg->integer = va_arg(*ap, int);
	    break; case STA_LONG:   arg->integer = va_arg(*ap, long);
	    break; case STA_LLONG:  arg->integer = va_arg(*ap, long long);
	    break; case STA_UCHAR:  arg->integer = (unsigned char)va_arg(*ap, int);
	    break; case STA_USHORT: arg->integer = (unsigned short)va_arg(*ap, int);
	    break; case STA_UINT:   arg->integer = va_arg(*ap, unsigned int);
	    break; case STA_ULONG:  arg->integer = va_arg(*ap, unsigned long);
	    break; case STA_ULLONG: arg->integer = va_arg(*ap, unsigned long long);
	    break; case STA_IMAX:   arg->integer = va_arg(*ap, intmax_t);
	    break; case STA_UMAX:   arg->integer = va_arg(*ap, uintmax_t);
	    break; case STA_SIZET:  arg->integer = va_arg(*ap, size_t);
	    break; case STA_PDIFF:  arg->integer = va_arg(*ap, ptrdiff_t);
	    break; case STA_UIPTR:  arg->integer = (uintptr_t)va_arg(*ap, void *);
	    break; case STA_PTR:    arg->pointer = va_arg(*ap, void *);
        break;
	}

    return true;
}

static int
vxprintf_getint(const char **str)
{
	int value;

	for (value = 0; isdigit(**str); ++*str) {
		if (value <= INT_MAX / 10 && **str - '0' <= INT_MAX - 10 * value)
            value = 10 * value + (**str - '0');
		else
            value = -1;
	}

	return value;
}

static inline
vxprintf_write(struct vxprintf_context *ctx,
               const char *str, size_t len)
{


}

static size_t
vxprintf_core(struct vxprintf_context *ctx,
              const char *fmt, va_list *args)
{
    enum vxprintf_flags flags, prev;
    enum vxprintf_state state;
    union vxprintf_arg arg;
    const char *scan, *end;
    size_t slen, count;
    int width, prec;
    bool xp;

	for (count = 0; *fmt; count += slen) {
        /* Handle literal text */
        scan = strchrnul(fmt, '%');

        /* Handle %% format specifiers */
        for (end = scan; scan[0] == '%'; scan += 2, ++end) {
            if (scan[1] != '%')
                break;
        }

        slen = end - fmt;
        if (slen) {
            vxprintf_write(ctx, fmt, slen);
            continue;
        }

        /* Read modifier flags */
        while (*fmt) switch (*fmt++) {
            case '#':
                flags |= FL_HASH;
                break;

            case '+':
                flags |= FL_PLUS;
                break;

            case '-':
                flags |= FL_MINUS;
                break;

            case '0':
                flags |= FL_ZERO;
                break;

            case ' ':
                flags |= FL_SPACE;
                break;

            case '\'':
                flags |= FL_TICK;
                break;

            default:
                break;
        }

        /* Read field width */
        if (fmt[0] == '*') {
            width = va_arg(*args, int);
            if (width < 0) {
                width = -width;
                flags |= FL_MINUS;
            }
            fmt++;
        } else {
            width = vxprintf_getint(&fmt);
            if (bfdev_unlikely(width < 0))
                return SIZE_MAX;
        }

		/* Read precision */
		if (fmt[0] == '.') {
            if (fmt[1] == '*') {
                prec = va_arg(*args, int);
                if (prec < 0)
                    prec = -1;
            } else {
                prec = vxprintf_getint(&fmt);
                if (bfdev_unlikely(prec < 0))
                    return SIZE_MAX;
            }
        } else {
			prec = -1;
			xp = 0;
        }

        /* Format specifier state machine */
        for (state = STA_BARE; state < STA_STOP; ++fmt) {
            if (bfdev_unlikely(OOB(*fmt)))
                return SIZE_MAX;
			prev = state;
			state = vxprintf_states[state][MAP(*fmt)];
        }

        if (bfdev_unlikely(state == STA_BARE))
            return SIZE_MAX;
        vxprintf_arg(&arg, state, args);

        switch (*fmt) {
            case 'n':
			    continue;

            case 'c': /* char */
                break;

            case 's': /* string */
                break;

            case 'p': /* pointer */
                bfdev_fallthrough;

            case 'x': case 'X': /* hex */
                goto decimal;

            case 'o': /* oct */
                bfdev_fallthrough;

            case 'd': case 'i': /* dec */
            decimal:
                goto number;

            case 'u':
            number:
                break;

            case 'C':
                bfdev_fallthrough;

            case 'S':
			    continue;

            case 'e': case 'f': case 'g': case 'a':
            case 'E': case 'F': case 'G': case 'A':
			    continue;

            default:
                return SIZE_MAX;
        }
    }

    return count;
}
