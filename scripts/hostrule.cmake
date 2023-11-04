# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

function(host_target target source)
    if(NOT HOST_C_COMPILER)
        add_executable(${target} ${source})
    else()
        add_custom_target(${target}
            COMMAND
            ${HOST_C_COMPILER} ${HOST_C_FLAGS}
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target} ${source}
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
endfunction()
