# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
# Copyright(c) 2023 Zhenlin Wang <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.12)
project(bfdev VERSION 1.0.7 LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckTypeSize)

set(BFDEV_NAME sirius)
set(BFDEV_VERSION ${PROJECT_VERSION})
set(BFDEV_EXTREVERSION -devel)

if(BFDEV_EXTREVERSION)
    string(APPEND BFDEV_VERSION ${BFDEV_EXTREVERSION})
endif()

set(BFDEV_ARCH generic)

set(BFDEV_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(BFDEV_HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(BFDEV_TEMPLATE_PATH ${PROJECT_SOURCE_DIR}/template)
set(BFDEV_PORT_PATH ${PROJECT_SOURCE_DIR}/port)
set(BFDEV_DOCUMENT_PATH ${PROJECT_SOURCE_DIR}/docs)
set(BFDEV_GENERATED_PATH ${PROJECT_BINARY_DIR}/generated)
set(BFDEV_EXPORT_PATH ${PROJECT_BINARY_DIR}/export)

set(BFDEV_ARCH_PATH ${PROJECT_SOURCE_DIR}/arch/${BFDEV_ARCH})
set(BFDEV_ARCH_HEADER_PATH ${BFDEV_ARCH_PATH}/include)
set(BFDEV_CONFIGURE ${BFDEV_GENERATED_PATH}/bfdev-config.cmake)

include(${PROJECT_SOURCE_DIR}/scripts/hostrule.cmake)
include(${PROJECT_SOURCE_DIR}/scripts/packed-header.cmake)
include(${PROJECT_SOURCE_DIR}/scripts/commit.cmake)

commit_hash(BFDEV_COMMITID)
commit_branch(BFDEV_BRANCH)
string(TIMESTAMP BFDEV_BUILD_TIME "%A %B %d %H:%M:%S UTC %Y" UTC)

string(APPEND BFDEV_RELEASE
    "${PROJECT_NAME}-${BFDEV_NAME} "
    "${BFDEV_VERSION}.${BFDEV_ARCH} "
    "${BFDEV_COMMITID}-${BFDEV_BRANCH} "
    "(${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} ${CMAKE_HOST_SYSTEM_NAME}) "
    "${BFDEV_BUILD_TIME}"
)

option(BFDEV_DEVEL "Enable development mode" OFF)
option(BFDEV_STRICT "Enable strict compilation" OFF)
option(BFDEV_EXAMPLES "Build examples" OFF)
option(BFDEV_TESTSUITE "Build testsuite" OFF)

option(BFDEV_BUILD_STATIC "Build Static Library" ON)
option(BFDEV_BUILD_SHARED "Build Shared Library" ON)
option(BFDEV_INSTALL_HEADERS "Install Headers" ON)
option(BFDEV_INSTALL_DOCS "Install Documentation" OFF)

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
option(BFDEV_DEBUG_MEMALLOC "Dynamic debug memalloc" ON)
option(BFDEV_CRC_EXTEND "CRC loop unfolding optimize" ON)

if(BFDEV_DEVEL)
    set(BFDEV_STRICT ON)
    set(BFDEV_EXAMPLES ON)
    set(BFDEV_TESTSUITE ON)
    set(BFDEV_INSTALL_DOCS ON)
    set(BFDEV_ASAN ON)
    set(BFDEV_UBSAN ON)
    set(BFDEV_GCOV ON)
endif()

include(${PROJECT_SOURCE_DIR}/build.cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/scripts)

packed_header(
    bfdev/template/
    _BFDEV_TEMPLATE_H_
    ${BFDEV_GENERATED_PATH}/bfdev/template.h
    ${BFDEV_HEADER_PATH}/bfdev/template
)

packed_header(
    bfdev/
    _BFDEV_H_
    ${BFDEV_GENERATED_PATH}/bfdev.h
    ${BFDEV_HEADER_PATH}/bfdev
    ${BFDEV_GENERATED_PATH}/bfdev
)

add_custom_target(
    bfdev_export
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${BFDEV_EXPORT_PATH}
        ${BFDEV_EXPORT_PATH}/bfdev
    COMMAND ${CMAKE_COMMAND} -E copy
        ${BFDEV_GENERATED_PATH}/bfdev.h
        ${BFDEV_EXPORT_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${BFDEV_HEADER_PATH}/bfdev
        ${BFDEV_ARCH_HEADER_PATH}/bfdev
        ${BFDEV_GENERATED_PATH}/bfdev
        ${BFDEV_EXPORT_PATH}/bfdev
)

set_property(
    GLOBAL PROPERTY
    "BFDEV_INCLUDE_DIRS"
    ${BFDEV_EXPORT_PATH}
)

macro(bfdev_dependencies target)
    add_dependencies(
        bfdev_export
        gen-crc8
        gen-crc16
        gen-crc16be
        gen-crc32
        gen-crc32be
        gen-crc64
        gen-crc64be
    )

    add_dependencies(
        ${target}
        bfdev_export
    )
endmacro()

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
     ${BFDEV_C_FLAGS}"
)

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
    if(BFDEV_BUILD_STATIC)
        add_library(bfdev_static STATIC ${BFDEV_LIBRARY})
        bfdev_dependencies(bfdev_static)

        set_target_properties(bfdev_static
            PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            LINKER_LANGUAGE C
        )

        install(TARGETS
            bfdev_static
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
    endif()

    if(BFDEV_BUILD_SHARED)
        add_library(bfdev_shared SHARED ${BFDEV_LIBRARY})
        bfdev_dependencies(bfdev_shared)

        set_target_properties(bfdev_shared
            PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            LINKER_LANGUAGE C
            MACOSX_RPATH ON
        )

        install(TARGETS
            bfdev_shared
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
    endif()

    if(BFDEV_INSTALL_HEADERS)
        install(FILES
            ${BFDEV_CONFIGURE}
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bfdev
        )

        install(FILES
            ${BFDEV_EXPORT_PATH}/bfdev.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

        install(DIRECTORY
            ${BFDEV_EXPORT_PATH}/bfdev
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()

    if(BFDEV_INSTALL_DOCS)
        install(FILES
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/AUTHORS
            ${PROJECT_SOURCE_DIR}/COPYING
            ${PROJECT_SOURCE_DIR}/COPYING.LIB
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
        )

        install(DIRECTORY
            ${BFDEV_DOCUMENT_PATH}
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
        )
    endif()
endif()
