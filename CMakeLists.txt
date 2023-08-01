# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.9)
project(bfdev VERSION "1.0" LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)
include(CheckCXXSymbolExists)
include(CheckFunctionExists)
include(CheckCSourceCompiles)

set(BFDEV_ARCH dummy)
set(BFDEV_EXTREVERSION -devel)

set(BFDEV_VERSION ${CMAKE_PROJECT_VERSION_MAJOR})
string(APPEND BFDEV_VERSION . ${CMAKE_PROJECT_VERSION_MINOR})

if(CMAKE_PROJECT_VERSION_PATCH)
    string(APPEND BFDEV_VERSION . ${CMAKE_PROJECT_VERSION_PATCH})
endif()

if(CMAKE_PROJECT_VERSION_TWEAK)
    string(APPEND BFDEV_VERSION . ${CMAKE_PROJECT_VERSION_TWEAK})
endif()

if(BFDEV_EXTREVERSION)
    string(APPEND BFDEV_VERSION ${BFDEV_EXTREVERSION})
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(BFDEV_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(BFDEV_DOCUMENT_PATH ${PROJECT_SOURCE_DIR}/docs)
set(BFDEV_GENERATED_PATH ${PROJECT_BINARY_DIR}/generated)

set(BFDEV_ARCH_PATH ${PROJECT_SOURCE_DIR}/arch/${BFDEV_ARCH})
set(BFDEV_ARCH_HEADER_PATH ${BFDEV_ARCH_PATH}/include)
set(BFDEV_ARCH_SOURCE_PATH ${BFDEV_ARCH_PATH}/src)

include(scripts/asm-generic.cmake)
include(scripts/commit.cmake)

commit_hash(BFDEV_COMMITID)
commit_branch(BFDEV_BRANCH)

option(ENABLE_DEVEL "Enable development mode" OFF)
option(ENABLE_EXAMPLES "Build examples" OFF)
option(ENABLE_DOCUMENT "Build document" OFF)
option(ENABLE_STRICT "Enable strict compilation" ON)
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_UBSAN "Enable Undefined Behaviour Sanitizer" OFF)

option(ENABLE_DBGLIST "Dynamic debug list" ON)
option(ENABLE_DBGSLIST "Dynamic debug slist" ON)
option(ENABLE_DBGHLIST "Dynamic debug hlist" ON)
option(ENABLE_DBGILIST "Dynamic debug ilist" ON)
option(ENABLE_DBGRBTREE "Dynamic debug rbtree" ON)

if(ENABLE_DEVEL)
    set(ENABLE_EXAMPLES ON)
    set(ENABLE_DOCUMENT ON)
    set(ENABLE_ASAN ON)
    set(ENABLE_UBSAN ON)
endif()

if(ENABLE_DOCUMENT)
    find_package(Doxygen REQUIRED)
    find_package(Sphinx REQUIRED breathe)

    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_XML YES)
    doxygen_add_docs(doxygen_docs ${PROJECT_SOURCE_DIR})

    sphinx_add_docs(
        htmldocs
        BREATHE_PROJECTS doxygen_docs
        BUILDER html
        SOURCE_DIRECTORY docs
    )

    sphinx_add_docs(
        latexdocs
        BREATHE_PROJECTS doxygen_docs
        BUILDER latex
        SOURCE_DIRECTORY docs
    )

    sphinx_add_docs(
        epubdocs
        BREATHE_PROJECTS doxygen_docs
        BUILDER epub
        SOURCE_DIRECTORY docs
    )

    sphinx_add_docs(
        xmldocs
        BREATHE_PROJECTS doxygen_docs
        BUILDER xml
        SOURCE_DIRECTORY docs
    )
endif()

asm_generic(
    bfdev/asm-generic/
    ${BFDEV_GENERATED_PATH}/bfdev/asm
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/generated)
add_subdirectory(scripts)

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

if(ENABLE_STRICT)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -Werror"
    )
endif()

if(ENABLE_ASAN)
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
endif()

if(ENABLE_DBGLIST)
    set(BFDEV_DEBUG_LIST ON)
endif()

if(ENABLE_DBGSLIST)
    set(BFDEV_DEBUG_SLIST ON)
endif()

if(ENABLE_DBGHLIST)
    set(BFDEV_DEBUG_HLIST ON)
endif()

if(ENABLE_DBGILIST)
    set(BFDEV_DEBUG_ILIST ON)
endif()

if(ENABLE_DBGRBTREE)
    set(BFDEV_DEBUG_RBTREE ON)
endif()

set(BFDEV_LIBRARY
    ${BFDEV_HEADER}
    ${BFDEV_ASM_HEADER}
    ${BFDEV_ARCH_ASM_HEADER}
    ${BFDEV_GENERATED_HEADER}
    ${BFDEV_INCLUDE}
    ${BFDEV_SOURCE}
    ${BFDEV_ARCH_SOURCE}
)

configure_file(
    ${CMAKE_MODULE_PATH}/config.h.in
    ${PROJECT_BINARY_DIR}/generated/bfdev/config.h
)

add_library(bfdev_object OBJECT ${BFDEV_LIBRARY})
add_library(bfdev_static STATIC ${BFDEV_LIBRARY})
add_library(bfdev_shared SHARED ${BFDEV_LIBRARY})
add_library(bfdev ALIAS bfdev_object)

macro(bfdev_dependencies target)
    add_dependencies(
        ${target}
        gen-crc16
        gen-crc32
        gen-crc64
    )
endmacro()

bfdev_dependencies(bfdev_object)
bfdev_dependencies(bfdev_static)
bfdev_dependencies(bfdev_shared)

if(ENABLE_EXAMPLES)
    enable_testing()
    add_subdirectory(examples)
endif()

set_target_properties(bfdev_shared
    PROPERTIES VERSION 1.0 SOVERSION 1
    OUTPUT_NAME bfdev
    MACOSX_RPATH ON
    LINKER_LANGUAGE C
)

set_target_properties(bfdev_static
    PROPERTIES
    OUTPUT_NAME bfdev
    LINKER_LANGUAGE C
)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(DIRECTORY
        ${BFDEV_HEADER_PATH}/bfdev
        ${BFDEV_GENERATED_PATH}/bfdev
        ${BFDEV_ARCH_HEADER_PATH}/bfdev
        DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(DIRECTORY
        ${BFDEV_DOCUMENT_PATH}
        DESTINATION
        "${CMAKE_INSTALL_DOCDIR}"
    )

    install(TARGETS
        bfdev_static bfdev_shared
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
