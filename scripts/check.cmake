# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 Zhenlin Wang <sanpeqf@gmail.com>
#

function(check_header_and_exit name)
    check_include_file(${name} ${name}_EXISTS)
    if(NOT ${name}_EXISTS)
        message(FATAL_ERROR "Header ${name} is required but not found.")
    endif()
endfunction()

function(check_symbol_and_exit header name)
    check_symbol_exists(${name} ${header} ${name}_EXISTS)
    if(NOT ${name}_EXISTS)
        message(FATAL_ERROR "Function ${name} is required but not found.")
    endif()
endfunction()

function(check_type_and_exit name size)
    string(REPLACE " " "_" safe_name "${name}")
    check_type_size("${name}" ${safe_name}_SIZE)
    if(NOT "${${safe_name}_SIZE}" STREQUAL "${size}")
        message(FATAL_ERROR "Type '${name}' size mismatch (expected ${size}, got ${${safe_name}_SIZE})")
    endif()
endfunction()
