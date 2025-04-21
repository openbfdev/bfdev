# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
     -std=gnu11 \
     -Wall \
     -Wextra \
     -Wno-override-init \
     -Wno-unused-parameter \
     -Wno-sign-compare \
     -Wno-pointer-sign \
     -Wno-null-pointer-arithmetic \
     -Wmissing-prototypes \
     -Wmissing-declarations \
     -fvisibility=hidden "
)

if(CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -Werror"
    )
endif()

include(${PROJECT_SOURCE_DIR}/scripts/check.cmake)
include(${PROJECT_SOURCE_DIR}/scripts/asm-generic.cmake)
include(${PROJECT_SOURCE_DIR}/scripts/sanitize.cmake)

asm_generic(
    bfdev/asm-generic/
    ${BFDEV_GENERATED_PATH}/bfdev/asm
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic
)

file(GLOB_RECURSE BFDEV_HEADER
    ${BFDEV_HEADER_PATH}/*.h
)

file(GLOB_RECURSE BFDEV_GENERATED_HEADER
    ${BFDEV_GENERATED_PATH}/*.h
)

file(GLOB_RECURSE BFDEV_ARCH_HEADER
    ${BFDEV_ARCH_HEADER_PATH}/*.h
)

set(BFDEV_INCLUDE_DIRS
    ${BFDEV_HEADER_PATH}
    ${BFDEV_GENERATED_PATH}
    ${BFDEV_ARCH_HEADER_PATH}
)

include_directories(${BFDEV_INCLUDE_DIRS})
include(${PROJECT_SOURCE_DIR}/scripts/platform.cmake)
include(${BFDEV_PORT_PATH}/build.cmake)

include(${BFDEV_ARCH_PATH}/build.cmake)
include(${BFDEV_SOURCE_PATH}/build.cmake)
include(${BFDEV_TEMPLATE_PATH}/build.cmake)

configure_file(
    ${BFDEV_MODULE_PATH}/config.h.in
    ${BFDEV_GENERATED_PATH}/bfdev/config.h
)

configure_file(
    ${BFDEV_MODULE_PATH}/bfdev-config.cmake.in
    ${BFDEV_CONFIGURE}
)

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
