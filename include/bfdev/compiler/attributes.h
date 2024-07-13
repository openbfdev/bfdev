/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 Fredrik Anderson <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_COMPILER_ATTRIBUTES_H_
#define _BFDEV_COMPILER_ATTRIBUTES_H_

#include <bfdev/config.h>

BFDEV_BEGIN_DECLS

/*
 * The attributes in this file are unconditionally defined and they directly
 * map to compiler attribute(s), unless one of the compilers does not support
 * the attribute. In that case, __has_attribute is used to check for support
 * and the reason is stated in its comment ("Optional: ...").
 */

/*
 * __has_attribute is supported on gcc >= 5, clang >= 2.9.
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
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-alias-function-attribute
 */
#define __bfdev_alias(symbol) __attribute__((__alias__(symbol)))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-aligned-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-aligned-type-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-aligned-variable-attribute
 */
#define __bfdev_aligned(x) __attribute__((__aligned__(x)))
#define __bfdev_aligned_largest __attribute__((__aligned__))

/*
 * Note: users of __bfdev_always_inline currently do not write "inline" themselves,
 * which seems to be required by gcc to apply the attribute according
 * to its docs (and also "warning: always_inline function might not be
 * inlinable [-Wattributes]" is emitted).
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-always_005finline-function-attribute
 * clang: mentioned
 */
#define __bfdev_always_inline inline __attribute__((__always_inline__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-assume_005faligned-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#assume-aligned
 */
#if __has_attribute(__assume_aligned__)
# define __bfdev_assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ## __VA_ARGS__)))
#else
# define __bfdev_assume_aligned(a, ...)
#endif

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-cold-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#index-cold-label-attribute
 */
#define __bfdev_cold __attribute__((__cold__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-cleanup-variable-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#cleanup
 */
#define __bfdev_cleanup(func) __attribute__((__cleanup__(func)))

/*
 * Note the long name.
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-const-function-attribute
 */
#define __bfdev_attribute_const __attribute__((__const__))

/*
 * Optional: only supported since gcc >= 9
 * Optional: not supported by clang
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-copy-function-attribute
 */
#if __has_attribute(__copy__)
# define __bfdev_copy(symbol) __attribute__((__copy__(symbol)))
#else
# define __bfdev_copy(symbol)
#endif

/*
 * Optional: only supported since gcc >= 14
 * Optional: only supported since clang >= 18
 *
 * gcc: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108896
 * clang: https://reviews.llvm.org/D148381
 */
#if __has_attribute(__counted_by__)
# define __bfdev_counted_by(member) __attribute__((__counted_by__(member)))
#else
# define __bfdev_counted_by(member)
#endif

/*
 * Optional: only supported since gcc >= 5.1
 * Optional: not supported by clang
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-designated_005finit-type-attribute
 */
#if __has_attribute(__designated_init__)
# define __bfdev_designated_init __attribute__((__designated_init__))
#else
# define __bfdev_designated_init
#endif

/*
 * Optional: not supported by clang
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-externally_005fvisible-function-attribute
 */
#if __has_attribute(__externally_visible__)
# define __bfdev_visible __attribute__((__externally_visible__))
#else
# define __bfdev_visible
#endif

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-format-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#format
 */
#define __bfdev_printf(a, b) __attribute__((__format__(printf, a, b)))
#define __bfdev_scanf(a, b) __attribute__((__format__(scanf, a, b)))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-nonnull-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#nonnull
 */
#define __bfdev_nonnull(nr, ...) __attribute__((__nonnull__(nr, ##__VA_ARGS__)))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-gnu_005finline-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#gnu-inline
 */
#define __bfdev_gnu_inline __attribute__((__gnu_inline__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute
 */
#define __bfdev_malloc __attribute__((__malloc__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-mode-type-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-mode-variable-attribute
 */
#define __bfdev_mode(x) __attribute__((__mode__(x)))

/*
 * Optional: only supported since gcc >= 7
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#index-no_005fcaller_005fsaved_005fregisters-function-attribute_002c-x86
 * clang: https://clang.llvm.org/docs/AttributeReference.html#no-caller-saved-registers
 */
#if __has_attribute(__no_caller_saved_registers__)
# define __bfdev_no_caller_saved_registers __attribute__((__no_caller_saved_registers__))
#else
# define __bfdev_no_caller_saved_registers
#endif

/*
 * Optional: not supported by clang
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noclone-function-attribute
 */
#if __has_attribute(__noclone__)
# define __bfdev_noclone __attribute__((__noclone__))
#else
# define __bfdev_noclone
#endif

/*
 * Add the pseudo keyword 'fallthrough' so case statement blocks
 * must end with any of these keywords:
 *   break;
 *   fallthrough;
 *   goto <label>;
 *   return [expression];
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes
 */
#if __has_attribute(__fallthrough__)
# define bfdev_fallthrough __attribute__((__fallthrough__))
#else
# define bfdev_fallthrough do {} while (0)  /* fallthrough */
#endif

/*
 * Note the missing underscores.
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noinline-function-attribute
 * clang: mentioned
 */
#define __bfdev_noinline __attribute__((__noinline__))

/*
 * Optional: only supported since gcc >= 8
 * Optional: not supported by clang
 *
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-nonstring-variable-attribute
 */
#if __has_attribute(__nonstring__)
# define __bfdev_nonstring __attribute__((__nonstring__))
#else
# define __bfdev_nonstring
#endif

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-functions-that-return-more-than-once
 */
#if __has_attribute(__returns_twice__)
# define __bfdev_returns_twice __attribute__((__returns_twice__))
#else
# define __bfdev_returns_twice
#endif

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-noreturn-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#noreturn
 * clang: https://clang.llvm.org/docs/AttributeReference.html#id1
 */
#define __bfdev_noreturn __attribute__((__noreturn__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-packed-type-attribute
 * clang: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-packed-variable-attribute
 */
#define __bfdev_packed __attribute__((__packed__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-pure-function-attribute
 */
#define __bfdev_pure __attribute__((__pure__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-section-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-section-variable-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#section-declspec-allocate
 */
#define __bfdev_section(section) __attribute__((__section__(section)))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-unused-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html#index-unused-type-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-unused-variable-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#index-unused-label-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#maybe-unused-unused
 */
#define __bfdev_always_unused __attribute__((__unused__))
#define __bfdev_maybe_unused __attribute__((__unused__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-used-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-used-variable-attribute
 */
#define __bfdev_used __attribute__((__used__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-warn_005funused_005fresult-function-attribute
 * clang: https://clang.llvm.org/docs/AttributeReference.html#nodiscard-warn-unused-result
 */
#define __bfdev_must_check __attribute__((__warn_unused_result__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-weak-function-attribute
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-weak-variable-attribute
 */
#define __bfdev_weak __attribute__((__weak__))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-visibility-function-attribute
 */
#define __bfdev_visibility(mode) __attribute__((visibility(mode)))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-constructor-function-attribute
 */
#define __bfdev_ctor __attribute__((constructor))

/*
 * gcc: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-destructor-function-attribute
 */
#define __bfdev_dtor __attribute__((destructor))

BFDEV_END_DECLS

#endif /* _BFDEV_COMPILER_ATTRIBUTES_H_ */
