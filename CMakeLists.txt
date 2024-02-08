# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.12)
project(bfdev VERSION 1.0.0 LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)

set(BFDEV_ARCH dummy)
set(BFDEV_NAME sirius)

set(BFDEV_EXTREVERSION -devel)
set(BFDEV_VERSION ${PROJECT_VERSION})

if(BFDEV_EXTREVERSION)
    string(APPEND BFDEV_VERSION ${BFDEV_EXTREVERSION})
endif()

set(BFDEV_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(BFDEV_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(BFDEV_DOCUMENT_PATH ${PROJECT_SOURCE_DIR}/docs)
set(BFDEV_GENERATED_PATH ${PROJECT_BINARY_DIR}/generated)

set(BFDEV_ARCH_PATH ${PROJECT_SOURCE_DIR}/arch/${BFDEV_ARCH})
set(BFDEV_ARCH_HEADER_PATH ${BFDEV_ARCH_PATH}/include)
set(BFDEV_CONFIGURE ${BFDEV_GENERATED_PATH}/bfdev-config.cmake)

include(scripts/asm-generic.cmake)
include(scripts/hostrule.cmake)
include(scripts/packed-header.cmake)
include(scripts/packed-source.cmake)
include(scripts/commit.cmake)

commit_hash(BFDEV_COMMITID)
commit_branch(BFDEV_BRANCH)

option(BFDEV_DEVEL "Enable development mode" OFF)
option(BFDEV_STRICT "Enable strict compilation" ON)
option(BFDEV_EXAMPLES "Build examples" OFF)
option(BFDEV_ASAN "Enable Address Sanitizer" OFF)
option(BFDEV_UBSAN "Enable Undefined Behaviour Sanitizer" OFF)

option(BFDEV_DEBUG_LIST "Dynamic debug list" ON)
option(BFDEV_DEBUG_SLIST "Dynamic debug slist" ON)
option(BFDEV_DEBUG_HLIST "Dynamic debug hlist" ON)
option(BFDEV_DEBUG_ILIST "Dynamic debug ilist" ON)
option(BFDEV_DEBUG_RBTREE "Dynamic debug rbtree" ON)
option(BFDEV_DEBUG_HEAP "Dynamic debug heap" ON)
option(BFDEV_DEBUG_REFCNT "Dynamic debug refcnt" ON)
option(BFDEV_CRC_EXTEND "CRC loop unfolding optimize" ON)

if(BFDEV_DEVEL)
    set(BFDEV_EXAMPLES ON)
    set(BFDEV_ASAN ON)
    set(BFDEV_UBSAN ON)
endif()

asm_generic(
    bfdev/asm-generic/
    ${BFDEV_GENERATED_PATH}/bfdev/asm
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic
)

configure_file(
    ${BFDEV_MODULE_PATH}/config.h.in
    ${BFDEV_GENERATED_PATH}/bfdev/config.h
)

configure_file(
    ${BFDEV_MODULE_PATH}/bfdev-config.cmake.in
    ${BFDEV_CONFIGURE}
)

add_compile_options(
    -std=gnu11
    -Wall
    -Wextra
    -Wno-override-init
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-pointer-sign
    -Wno-null-pointer-arithmetic
    -Wmissing-prototypes
    -Wmissing-declarations
    -fvisibility=hidden
)

if(BFDEV_STRICT)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -Werror"
    )
endif()

if(BFDEV_ASAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=address \
         -fsanitize=undefined \
         -fsanitize-recover=all \
         -fno-omit-frame-pointer \
         -fno-stack-protector"
    )
    if(NOT APPLE)
        set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} \
             -fsanitize=leak"
        )
    endif()
endif()

if(BFDEV_UBSAN)
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
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/generated)

add_subdirectory(${PROJECT_SOURCE_DIR}/scripts)
include(${PROJECT_SOURCE_DIR}/build.cmake)

set(BFDEV_LIBRARY_HEADER
    ${BFDEV_HEADER}
    ${BFDEV_ASM_HEADER}
    ${BFDEV_ARCH_ASM_HEADER}
    ${BFDEV_GENERATED_HEADER}
)

set(BFDEV_LIBRARY_SOURCE
    ${BFDEV_SOURCE}
    ${BFDEV_ARCH_SOURCE}
)

set(BFDEV_LIBRARY
    ${BFDEV_LIBRARY_HEADER}
    ${BFDEV_LIBRARY_SOURCE}
)

macro(bfdev_dependencies target)
    add_dependencies(
        ${target}
        gen-crc8
        gen-crc16
        gen-crc16be
        gen-crc32
        gen-crc32be
        gen-crc64
        gen-crc64be
    )
endmacro()

packed_header(
    bfdev/
    _BFDEV_H_
    ${BFDEV_GENERATED_PATH}/bfdev.h
    ${BFDEV_HEADER_PATH}/bfdev
)

packed_source(
    ${PROJECT_BINARY_DIR}/bfdev.c
    "${BFDEV_LIBRARY_SOURCE}"
    "#undef MODULE_NAME\n"
    "#undef bfdev_log_fmt\n"
)

add_library(bfdev_object OBJECT ${BFDEV_LIBRARY})
bfdev_dependencies(bfdev_object)
add_library(bfdev ALIAS bfdev_object)

if(BFDEV_EXAMPLES)
    enable_testing()
    add_subdirectory(examples)
endif()

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    add_library(bfdev_static STATIC ${BFDEV_LIBRARY})
    add_library(bfdev_shared SHARED ${BFDEV_LIBRARY})

    bfdev_dependencies(bfdev_static)
    bfdev_dependencies(bfdev_shared)

    set_target_properties(bfdev_static
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        LINKER_LANGUAGE C
    )

    set_target_properties(bfdev_shared
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        LINKER_LANGUAGE C
        MACOSX_RPATH ON
    )

    install(FILES
        ${BFDEV_GENERATED_PATH}/bfdev.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(FILES
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/AUTHORS
        ${PROJECT_SOURCE_DIR}/COPYING
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    install(FILES
        ${BFDEV_CONFIGURE}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bfdev
    )

    install(DIRECTORY
        ${BFDEV_HEADER_PATH}/bfdev
        ${BFDEV_GENERATED_PATH}/bfdev
        ${BFDEV_ARCH_HEADER_PATH}/bfdev
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY
        ${BFDEV_DOCUMENT_PATH}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    install(TARGETS
        bfdev_static bfdev_shared
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
