# SPDX-License-Identifier: LGPL-3.0-or-later
#
# Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
#

if(BFDEV_PORT_TYPE)
    include(${BFDEV_PORT_PATH}/${BFDEV_PORT_TYPE}/build.cmake)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/tests/build.cmake)
