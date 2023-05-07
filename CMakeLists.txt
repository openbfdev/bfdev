# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.6)
project(bfdev LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)
include(CheckCXXSymbolExists)
include(CheckFunctionExists)
include(CheckCSourceCompiles)

set(BFDEV_ARCH dummy)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(BFDEV_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(BFDEV_GENERATED_PATH ${PROJECT_SOURCE_DIR}/generated)
set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)

set(BFDEV_ARCH_PATH ${PROJECT_SOURCE_DIR}/arch/${BFDEV_ARCH})
set(BFDEV_ARCH_HEADER_PATH ${BFDEV_ARCH_PATH}/include)
set(BFDEV_ARCH_SOURCE_PATH ${BFDEV_ARCH_PATH}/src)

macro(asmgeneric prefix generated compare source)
    file(GLOB srclist ${source}/*)
    file(GLOB cmplist ${compare}/*)

    foreach(srcpath ${srclist})
        string(REGEX REPLACE ".+/(.+)" "\\1" filename ${srcpath})
        list(FIND cmplist ${compare}/${filename} retval)
        set(genfile ${generated}/${filename})

        file(REMOVE ${genfile})
        if(${retval})
            file(WRITE ${genfile}
                "/*\n"
                " * Automatically generated file; DO NOT EDIT.\n"
                " * " ${CMAKE_PROJECT_NAME} " asm-generic\n"
                " */\n"
                "#include <" ${prefix} ${filename} ">\n"
            )
        endif(${retval})
    endforeach(srcpath)

    unset(srclist)
    unset(cmplist)
endmacro(asmgeneric)

asmgeneric(
    bfdev/asm-generic/
    generated/bfdev/asm
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic
)

file(GLOB BFDEV_HEADER
    ${BFDEV_HEADER_PATH}/bfdev/*.h
)

file(GLOB BFDEV_ASM_HEADER
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic/*.h
)

file(GLOB BFDEV_ARCH_ASM_HEADER
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm/*.h
)

file(GLOB BFDEV_GENERATED_HEADER
    ${BFDEV_GENERATED_PATH}/*.h
)

file(GLOB_RECURSE BFDEV_SOURCE
    ${BFDEV_SOURCE_PATH}/*.c
)

file(GLOB_RECURSE BFDEV_ARCH_SOURCE
    ${BFDEV_ARCH_SOURCE_PATH}/*.c
)

add_definitions(
    " -std=gnu17"
    " -Wall"
    " -Wextra"
    " -Wno-override-init"
    " -Wno-unused-parameter"
    " -Wno-sign-compare"
    " -Wno-pointer-sign"
    " -Wno-null-pointer-arithmetic"
    " -fvisibility=hidden"
)

option(ENABLE_STRICT "Enable strict compilation" ON)
if(ENABLE_STRICT)
    add_definitions(
        "-Werror "
    )
endif(ENABLE_STRICT)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=address -fsanitize=undefined \
         -fsanitize=leak -fsanitize-recover=all \
         -fno-omit-frame-pointer -fno-stack-protector"
    )
endif(ENABLE_ASAN)

option(ENABLE_UBSAN "Enable Undefined Behaviour Sanitizer" OFF)
if(ENABLE_UBSAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=alignment \
         -fsanitize=bounds \
         -fsanitize=shift \
         -fsanitize=integer-divide-by-zero \
         -fsanitize=unreachable \
         -fsanitize=bool \
         -fsanitize=enum \
         -fsanitize-undefined-trap-on-error"
    )
endif(ENABLE_UBSAN)

option(ENABLE_DBGLIST "Dynamic debug list" OFF)
if(ENABLE_DBGLIST)
    set(BFDEV_DEBUG_LIST ON)
    set(BFDEV_SOURCE
        src/list-debug.c
        ${BFDEV_SOURCE}
    )
endif(ENABLE_DBGLIST)

option(ENABLE_DBGSLIST "Dynamic debug slist" OFF)
if(ENABLE_DBGSLIST)
    set(BFDEV_DEBUG_SLIST ON)
    set(BFDEV_SOURCE
        src/slist-debug.c
        ${BFDEV_SOURCE}
    )
endif(ENABLE_DBGSLIST)

option(ENABLE_DBGHLIST "Dynamic debug hlist" OFF)
if(ENABLE_DBGHLIST)
    set(BFDEV_DEBUG_HLIST ON)
    set(BFDEV_SOURCE
        src/hlist-debug.c
        ${BFDEV_SOURCE}
    )
endif(ENABLE_DBGHLIST)

option(ENABLE_DBGRBTREE "Dynamic debug rbtree" OFF)
if(ENABLE_DBGRBTREE)
    set(BFDEV_DEBUG_RBTREE ON)
    set(BFDEV_SOURCE
        src/rbtree-debug.c
        ${BFDEV_SOURCE}
    )
endif(ENABLE_DBGRBTREE)

set(BFDEV_LIBRARY
    ${BFDEV_HEADER}
    ${BFDEV_ASM_HEADER}
    ${BFDEV_ARCH_ASM_HEADER}
    ${BFDEV_GENERATED_HEADER}
    ${BFDEV_INCLUDE}
    ${BFDEV_SOURCE}
    ${BFDEV_ARCH_SOURCE}
)

CHECK_C_SOURCE_COMPILES(
"
    int main(void) {
        long  n = 0;
        if (!__sync_bool_compare_and_swap(&n, 0, 3))
            return 1;
        if (__sync_fetch_and_add(&n, 1) != 3)
            return 1;
        if (__sync_lock_test_and_set(&n, 5) != 4)
            return 1;
        if (n != 5)
            return 1;
        if (__sync_or_and_fetch(&n, 2) != 7)
            return 1;
        if (__sync_and_and_fetch(&n, 5) != 5)
            return 1;
        __sync_lock_release(&n);
        if (n != 0)
            return 1;
        return 0;
    }
"
BFDEV_HAVE_GCC_ATOMIC
)

configure_file(
    ${CMAKE_MODULE_PATH}/config.h.in
    ${PROJECT_BINARY_DIR}/generated/bfdev/config.h
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/generated)

add_library(bfdev_shared SHARED ${BFDEV_LIBRARY})
add_library(bfdev_static STATIC ${BFDEV_LIBRARY})

SET_TARGET_PROPERTIES(bfdev_shared
    PROPERTIES VERSION 1.0 SOVERSION 1
    OUTPUT_NAME bfdev
    MACOSX_RPATH ON
    LINKER_LANGUAGE C
)

SET_TARGET_PROPERTIES(bfdev_static
    PROPERTIES
    OUTPUT_NAME bfdev
    LINKER_LANGUAGE C
)

install(DIRECTORY
    ${BFDEV_HEADER_PATH}/bfdev
    ${BFDEV_GENERATED_PATH}/bfdev
    ${BFDEV_ARCH_HEADER_PATH}/bfdev
    DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(FILES
    docs/logo.svg
    DESTINATION
    "${CMAKE_INSTALL_DOCDIR}"
)

install(TARGETS
    bfdev_static bfdev_shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_library(bfdev ALIAS bfdev_shared)
add_subdirectory(examples)
