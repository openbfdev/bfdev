# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.12)
project(bfdev VERSION 1.0.3 LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)

set(BFDEV_NAME sirius)
set(BFDEV_VERSION ${PROJECT_VERSION})
set(BFDEV_EXTREVERSION -stable)

if(BFDEV_EXTREVERSION)
    string(APPEND BFDEV_VERSION ${BFDEV_EXTREVERSION})
endif()

set(BFDEV_ARCH generic)

set(BFDEV_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(BFDEV_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(BFDEV_DOCUMENT_PATH ${PROJECT_SOURCE_DIR}/docs)
set(BFDEV_GENERATED_PATH ${PROJECT_BINARY_DIR}/generated)

set(BFDEV_ARCH_PATH ${PROJECT_SOURCE_DIR}/arch/${BFDEV_ARCH})
set(BFDEV_ARCH_HEADER_PATH ${BFDEV_ARCH_PATH}/include)
set(BFDEV_CONFIGURE ${BFDEV_GENERATED_PATH}/bfdev-config.cmake)

include(scripts/hostrule.cmake)
include(scripts/packed-header.cmake)
include(scripts/packed-source.cmake)
include(scripts/commit.cmake)

commit_hash(BFDEV_COMMITID)
commit_branch(BFDEV_BRANCH)

option(BFDEV_DEVEL "Enable development mode" OFF)
option(BFDEV_STRICT "Enable strict compilation" ON)
option(BFDEV_EXAMPLES "Build examples" OFF)
option(BFDEV_TESTSUITE "Build testsuite" OFF)

option(BFDEV_ASAN "Enable Address Sanitizer" OFF)
option(BFDEV_UBSAN "Enable Undefined Behaviour Sanitizer" OFF)
option(BFDEV_GCOV "Enable Code Coverage Test" OFF)

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
    set(BFDEV_TESTSUITE ON)
    set(BFDEV_ASAN ON)
    set(BFDEV_UBSAN ON)
    set(BFDEV_GCOV ON)
endif()

include(${PROJECT_SOURCE_DIR}/build.cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/scripts)

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

add_library(bfdev_object OBJECT ${BFDEV_LIBRARY})
bfdev_dependencies(bfdev_object)
add_library(bfdev ALIAS bfdev_object)

if(BFDEV_EXAMPLES)
    enable_testing()
    add_subdirectory(examples)
endif()

if(BFDEV_TESTSUITE)
    enable_testing()
    add_subdirectory(testsuite)
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
