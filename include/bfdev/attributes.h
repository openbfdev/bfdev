/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ATTRIBUTES_H_
#define _BFDEV_ATTRIBUTES_H_

#include <bfdev/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The attributes in this file are unconditionally defined and they directly
 * map to compiler attribute(s), unless one of the compilers does not support
 * the attribute. In that case, __has_attribute is used to check for support
 * and the reason is stated in its comment ("Optional: ...").
 *
 * Any other "attributes" (i.e. those that depend on a configuration option,
 * on a compiler, on an architecture, on plugins, on other attributes...)
 * should be defined elsewhere (e.g. compiler_types.h or compiler-*.h).
 * The intention is to keep this file as simple as possible, as well as
 * compiler- and version-agnostic (e.g. avoiding GCC_VERSION checks).
 *
 * This file is meant to be sorted (by actual attribute name,
 * not by #define identifier). Use the __attribute__((__name__)) syntax
 * (i.e. with underscores) to avoid future collisions with other macros.
 * Provide links to the documentation of each supported compiler, if it exists.
 */

/*
 * __has_attribute is supported on gcc >= 5, clang >= 2.9 and icc >= 17.
 * In the meantime, to support gcc < 5, we implement __has_attribute
 * by hand.
 */
#ifndef __has_attribute
# define __has_attribute(x) __GCC4_has_attribute_##x
# define __GCC4_has_attribute___assume_aligned__ (__GNUC_MINOR__ >= 9)
# define __GCC4_has_attribute___copy__ 0
# define __GCC4_has_attribute___designated_init__ 0
# define __GCC4_has_attribute___externally_visible__ 1
# define __GCC4_has_attribute___no_caller_saved_registers__ 0
# define __GCC4_has_attribute___noclone__ 1
# define __GCC4_has_attribute___nonstring__ 0
# define __GCC4_has_attribute___no_sanitize_address__ (__GNUC_MINOR__ >= 8)
# define __GCC4_has_attribute___no_sanitize_undefined__ (__GNUC_MINOR__ >= 9)
# define __GCC4_has_attribute___fallthrough__ 0
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-alias-function-attribute
 */
#ifndef __alias
# define __alias(symbol) __attribute__((__alias__(symbol)))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-aligned-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-aligned-type-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-aligned-variable-attribute
 */
#ifndef __aligned
# define __aligned(x) __attribute__((__aligned__(x)))
# define __aligned_largest __attribute__((__aligned__))
#endif

/*
 * Note: users of __always_inline currently do not write "inline" themselves,
 * which seems to be required by gcc to apply the attribute according
 * to its docs (and also "warning: always_inline function might not be
 * inlinable [-Wattributes]" is emitted).
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-always_005finline-function-attribute
 * clang: mentioned
 */
#ifndef __always_inline
# define __always_inline inline __attribute__((__always_inline__))
#endif

/*
 * The second argument is optional (default 0), so we use a variadic macro
 * to make the shorthand.
 *
 * Beware: Do not apply this to functions which may return
 * ERR_PTRs. Also, it is probably unwise to apply it to functions
 * returning extra information in the low bits (but in that case the
 * compiler should see some alignment anyway, when the return value is
 * massaged by 'flags = ptr & 3; ptr &= ~3;').
 *
 * Optional: only supported since gcc >= 4.9
 * Optional: not supported by icc
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-assume_005faligned-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#assume-aligned
 */
#ifndef __assume_aligned
# if __has_attribute(__assume_aligned__)
#  define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ## __VA_ARGS__)))
# else
#  define __assume_aligned(a, ...)
# endif
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-cold-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#index-cold-label-attribute
 */
#ifndef __cold
# define __cold __attribute__((__cold__))
#endif

/*
 * Note the long name.
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-const-function-attribute
 */
#ifndef __attribute_const__
# define __attribute_const__ __attribute__((__const__))
#endif

/*
 * Optional: only supported since gcc >= 9
 * Optional: not supported by clang
 * Optional: not supported by icc
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-copy-function-attribute
 */
#ifndef __copy
# if __has_attribute(__copy__)
#  define __copy(symbol) __attribute__((__copy__(symbol)))
# else
#  define __copy(symbol)
# endif
#endif

/*
 * Don't. Just don't. See commit 771c035372a0 ("deprecate the '__deprecated'
 * attribute warnings entirely and for good") for more information.
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-deprecated-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-deprecated-type-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-deprecated-variable-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Enumerator-Attributes.html#index-deprecated-enumerator-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#deprecated
 */
#ifndef __deprecated
# define __deprecated
#endif

/*
 * Optional: only supported since gcc >= 5.1
 * Optional: not supported by clang
 * Optional: not supported by icc
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-designated_005finit-type-attribute
 */
#ifndef __designated_init
# if __has_attribute(__designated_init__)
#  define __designated_init __attribute__((__designated_init__))
# else
#  define __designated_init
# endif
#endif

/*
 * Optional: not supported by clang
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-externally_005fvisible-function-attribute
 */
#ifndef __visible
# if __has_attribute(__externally_visible__)
#  define __visible __attribute__((__externally_visible__))
# else
#  define __visible
# endif
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-format-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#format
 */
#ifndef __printf
# define __printf(a, b) __attribute__((__format__(printf, a, b)))
# define __scanf(a, b) __attribute__((__format__(scanf, a, b)))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-nonnull-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#nonnull
 */
#ifndef __nonnull
# define __nonnull(nr, ...) __attribute__((__nonnull__(nr, ##__VA_ARGS__)))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-gnu_005finline-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#gnu-inline
 */
#ifndef __gnu_inline
# define __gnu_inline __attribute__((__gnu_inline__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute
 */
#ifndef __malloc
# define __malloc __attribute__((__malloc__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-mode-type-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-mode-variable-attribute
 */
#ifndef __mode
# define __mode(x) __attribute__((__mode__(x)))
#endif

/*
 * Optional: only supported since gcc >= 7
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#index-no_005fcaller_005fsaved_005fregisters-function-attribute_002c-x86
 * clang: https://clang.llvm.org/docs/AttributeReference.html#no-caller-saved-registers
 */
#ifndef __no_caller_saved_registers
# if __has_attribute(__no_caller_saved_registers__)
#  define __no_caller_saved_registers __attribute__((__no_caller_saved_registers__))
# else
#  define __no_caller_saved_registers
# endif
#endif

/*
 * Optional: not supported by clang
 *
 *  gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noclone-function-attribute
 */
#ifndef __noclone
# if __has_attribute(__noclone__)
#  define __noclone __attribute__((__noclone__))
# else
#  define __noclone
# endif
#endif

/*
 * Add the pseudo keyword 'fallthrough' so case statement blocks
 * must end with any of these keywords:
 *   break;
 *   fallthrough;
 *   goto <label>;
 *   return [expression];
 *
 *  gcc: https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes
 */
#ifndef fallthrough
# if __has_attribute(__fallthrough__)
#  define fallthrough __attribute__((__fallthrough__))
# else
#  define fallthrough do {} while (0)  /* fallthrough */
# endif
#endif

/*
 * Note the missing underscores.
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noinline-function-attribute
 * clang: mentioned
 */
#ifndef noinline
# define noinline __attribute__((__noinline__))
#endif

/*
 * Optional: only supported since gcc >= 8
 * Optional: not supported by clang
 * Optional: not supported by icc
 *
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-nonstring-variable-attribute
 */
#ifndef __nonstring
# if __has_attribute(__nonstring__)
#  define __nonstring __attribute__((__nonstring__))
# else
#  define __nonstring
# endif
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-functions-that-return-more-than-once
 */
#ifndef __returns_twice
# if __has_attribute(__returns_twice__)
#  define __returns_twice __attribute__((__returns_twice__))
# else
#  define __returns_twice
# endif
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noreturn-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#noreturn
 * clang: https://clang.llvm.org/docs/AttributeReference.html#id1
 */
#ifndef __noreturn
# define __noreturn __attribute__((__noreturn__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-packed-type-attribute
 * clang: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-packed-variable-attribute
 */
#ifndef __packed
# define __packed __attribute__((__packed__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-pure-function-attribute
 */
#ifndef __pure
# define __pure __attribute__((__pure__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-section-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-section-variable-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#section-declspec-allocate
 */
#ifndef __section
# define __section(section) __attribute__((__section__(section)))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-unused-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-unused-type-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-unused-variable-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#index-unused-label-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#maybe-unused-unused
 */
#ifndef __always_unused
# define __always_unused __attribute__((__unused__))
# define __maybe_unused __attribute__((__unused__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-used-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-used-variable-attribute
 */
#ifndef __used
# define __used __attribute__((__used__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-weak-function-attribute
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-weak-variable-attribute
 */
#ifndef __weak
# define __weak __attribute__((__weak__))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-visibility-function-attribute
 */
#ifndef __visibility
# define __visibility(mode) __attribute__((visibility(mode)))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-constructor-function-attribute
 */
#ifndef __ctor
# define __ctor __attribute__((constructor))
#endif

/*
 *   gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-destructor-function-attribute
 */
#ifndef __dtor
# define __dtor __attribute__((destructor))
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ATTRIBUTES_H_ */
